#include "sensor_bus1.h"
#include "main.h"
#include "stm32f7xx_hal.h"
#include "general_purpose_defs.h"
#include "debug_analyzer.h"
#include <stm32f7xx_nucleo_bus.h>
#include <stm32f7xx_hal_i2c.h>
#include <stdint.h>
#include <SEGGER_SYSVIEW.h>

typedef enum {
	DMA_RDY = 0, DMA_MAG = 1, DMA_ACC_GYRO = 2
} DMA_BUS_t;


#define DMA_QUEUE_SIZE 5

static volatile DMA_BUS_t in_process_dma = DMA_RDY;
static volatile GP_Status_t req_mag = GP_FALSE;
static volatile GP_Status_t req_gyro_acc = GP_FALSE;
static volatile GP_Status_t lock_update = GP_FALSE;

static GYRO_ACC_t acc_gyro_raw_data1[GYRO_ACC_SAMPLES];
static GYRO_ACC_t acc_gyro_raw_data2[GYRO_ACC_SAMPLES];
static GYRO_ACC_t * p_acc_gyro_raw_data = NULL;
static uint16_t acc_gyro_raw_data_size = sizeof(GYRO_ACC_t) * GYRO_ACC_SAMPLES;
static LSM303AGR_AxesRaw_t mag_raw_data1;
static LSM303AGR_AxesRaw_t mag_raw_data2;
static LSM303AGR_AxesRaw_t * p_mag_raw_data = NULL;
static AccGyroCallback_t pAccGyroCallback = NULL;
static MagCallback_t pMagCallback = NULL;

static void run_pending_dma(void);


void SensorBus1_Init(AccGyroCallback_t pAccGyroCB, MagCallback_t pMagCB) {

	pAccGyroCallback = pAccGyroCB;
	pMagCallback = pMagCB;

	req_mag = GP_FALSE;
	req_gyro_acc = GP_FALSE;
	lock_update = GP_FALSE;
	in_process_dma = DMA_RDY;
	p_acc_gyro_raw_data = &acc_gyro_raw_data1[0];
	p_mag_raw_data = &mag_raw_data1;

	bsp_lsm6dsl_init();
	bsp_lsm303agr_init();
}


void SensorBus1_Start(void) {
	bsp_lsm6dsl_enable();
	bsp_lsm303agr_enable();
}


void SensorBus1_Intr(uint16_t GPIO_Pin) {
	SEGGER_SYSVIEW_PrintfHost("SensorBus1_Intr");

	// If acc/gyro
	if (GPIO_Pin == Lms6dsl_Int1_Pin) {
		req_gyro_acc = GP_TRUE;
		SEGGER_SYSVIEW_PrintfHost("req_gyro_acc");
	}

	// if mag
	if (GPIO_Pin == MagDataRdy_Pin) {
		req_mag = GP_TRUE;
		SEGGER_SYSVIEW_PrintfHost("req_mag");
	}
	run_pending_dma();
}


void run_pending_dma(void) {
	SEGGER_SYSVIEW_PrintfHost("run_pending_dma");
	// Must not be reentrant, this will catch that
	// Must have SensorBus1_DMA_CallBack and SensorBus1_Intr on same interrupt level
	assert_param(lock_update == GP_FALSE);
	lock_update = GP_TRUE;

	if (in_process_dma != DMA_RDY) { // exit if previous still running
		lock_update = GP_FALSE;
		return;
	}

	// Else check for ryro/acc DMA read
	if (req_gyro_acc) {
		// TODO: Handle error
		HAL_I2C_Mem_Read_DMA(&hi2c1, LSM6DSL_I2C_ADD_H, LSM6DSL_FIFO_DATA_OUT_L,
				I2C_MEMADD_SIZE_8BIT, (uint8_t*) p_acc_gyro_raw_data,
				acc_gyro_raw_data_size);

		in_process_dma = DMA_ACC_GYRO;
		req_gyro_acc = GP_FALSE;
		SEGGER_SYSVIEW_PrintfHost("req_gyro_acc DMA");
	// Else check for mag request
	} else if (req_mag) {
		// TODO: Handle error
		HAL_I2C_Mem_Read_DMA(&hi2c1, LSM303AGR_I2C_ADD_MG,
				LSM303AGR_OUTX_L_REG_M, I2C_MEMADD_SIZE_8BIT,
				(uint8_t*) p_mag_raw_data, sizeof(LSM303AGR_AxesRaw_t));


		in_process_dma = DMA_MAG;
		req_mag = GP_FALSE;
		SEGGER_SYSVIEW_PrintfHost("req_mag DMA");
	}

	lock_update = GP_FALSE;
}


void SensorBus1_DMA_CallBack(void) {

	DebugExtAnalyzerPulse1();

	// Handle data ready from last dma request
	switch (in_process_dma) {

	case DMA_ACC_GYRO:
		SEGGER_SYSVIEW_PrintfHost("DMA_ACC_GYRO CALLBACK");
			// Acc Gyro call back with current raw data
		pAccGyroCallback(p_acc_gyro_raw_data, GYRO_ACC_SAMPLES);

		// Swap raw data storage
		if (p_acc_gyro_raw_data == &acc_gyro_raw_data1[0]) {
			p_acc_gyro_raw_data = &acc_gyro_raw_data2[0];
		} else {
			p_acc_gyro_raw_data = &acc_gyro_raw_data1[0];
		}

		// Check if we have back to back gyro/acc interrupts.
		// This can happen if we halt for debug.
		// In this case the interrupt line remains high between data sets and
		// therefore does not generate second rising edge.
		// We'll just check for interrupt still high and schedule DMA again
		if (HAL_GPIO_ReadPin(Lms6dsl_Int1_GPIO_Port, Lms6dsl_Int1_Pin) == GPIO_PIN_SET) {
			req_gyro_acc = GP_TRUE;
			SEGGER_SYSVIEW_PrintfHost("Reschedule req_gyro_acc");
		}

		break;

	case DMA_MAG:
		SEGGER_SYSVIEW_PrintfHost("DMA_MAG CALLBACK");
		// Mag call back with current raw data
		pMagCallback(p_mag_raw_data, 1);

		// Swap raw data storage
		if (p_mag_raw_data == &mag_raw_data1) {
			p_mag_raw_data = &mag_raw_data2;
		} else {
			p_mag_raw_data = &mag_raw_data1;
		}

		// Check if we have back to back gyro ready interrupts.
		// This can happen if we halt for debug.
		// In this case the interrupt line remains high between data sets and
		// therefore does not generate second rising edge.
		// We'll just check for interrupt still high and schedule DMA again
		if (HAL_GPIO_ReadPin(MagDataRdy_GPIO_Port, MagDataRdy_Pin) == GPIO_PIN_SET) {
			req_mag = GP_TRUE;
			SEGGER_SYSVIEW_PrintfHost("Reschedule req_mag");
		}

		break;

	default:
		assert_param(!HAL_OK);
		break;
	}

	in_process_dma = DMA_RDY; // Indicate ready for more

	// Schedule next pending
	run_pending_dma();
}





