#include "sensor_bus1.h"
#include "main.h"
#include "stm32f7xx_hal.h"
#include "general_purpose_defs.h"
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


void InitSensorBus1(AccGyroCallback_t pAccGyroCB, MagCallback_t pMagCB) {

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
	SEGGER_SYSVIEW_PrintfHost("InitSensorBus1");
}


void StartSensorBus1(void) {
	bsp_lsm6dsl_enable();
	bsp_lsm303agr_enable();
	SEGGER_SYSVIEW_PrintfHost("StartSensorBus1");
}


void UpdateSensorBus1(void) {

	// Set based on pin state, using this method help with missed updates when stopped for debug
	if (HAL_GPIO_ReadPin(MagDataRdy_GPIO_Port, MagDataRdy_Pin) == GPIO_PIN_SET) {
		req_mag = GP_TRUE;
		SEGGER_SYSVIEW_PrintfHost("req_mag requested");
	}


	if (HAL_GPIO_ReadPin(Lms6dsl_Int1_GPIO_Port, Lms6dsl_Int1_Pin) == GPIO_PIN_SET) {
		req_gyro_acc = GP_TRUE;
		SEGGER_SYSVIEW_PrintfHost("req_gyro_acc requested");
	}

	run_pending_dma();
}


void run_pending_dma(void) {

	// Must not be reentrant, this will catch that
	// Must have SensorBus1_DMA_CallBack and UpdateSensorBus1 on same interrupt level
	assert_param(lock_update == GP_FALSE);
	lock_update = GP_TRUE;

	if (in_process_dma != DMA_RDY) { // exit if previous still running
		SEGGER_SYSVIEW_PrintfHost("run_pending_dma busy");
		lock_update = GP_FALSE;
		return;
	}


	// Else check for ryro/acc DMA read
	if (req_gyro_acc) {
		assert_param(HAL_I2C_Mem_Read_DMA(&hi2c1, LSM6DSL_I2C_ADD_H, LSM6DSL_FIFO_DATA_OUT_L,
				I2C_MEMADD_SIZE_8BIT, (uint8_t*) p_acc_gyro_raw_data,
				acc_gyro_raw_data_size) == HAL_OK);

		SEGGER_SYSVIEW_PrintfHost("req_gyro_acc HAL_I2C_Mem_Read_DMA scheduled");

		in_process_dma = DMA_ACC_GYRO;
		req_gyro_acc = GP_FALSE;

	// Else check for mag request
	} else if (req_mag) {
		assert_param(HAL_I2C_Mem_Read_DMA(&hi2c1, LSM303AGR_I2C_ADD_MG,
				LSM303AGR_OUTX_L_REG_M, I2C_MEMADD_SIZE_8BIT,
				(uint8_t*) p_mag_raw_data, sizeof(LSM303AGR_AxesRaw_t)) == HAL_OK);

		SEGGER_SYSVIEW_PrintfHost("req_mag HAL_I2C_Mem_Read_DMA scheduled");

		in_process_dma = DMA_MAG;
		req_mag = GP_FALSE;
	}

	lock_update = GP_FALSE;
}


void SensorBus1_DMA_CallBack(void) {


	// Handle data ready from last dma request
	switch (in_process_dma) {

	case DMA_ACC_GYRO:
		// Acc Gyro call back with current raw data
		pAccGyroCallback(p_acc_gyro_raw_data, GYRO_ACC_SAMPLES);

		// Swap raw data storage
		if (p_acc_gyro_raw_data == &acc_gyro_raw_data1[0]) {
			p_acc_gyro_raw_data = &acc_gyro_raw_data2[0];
		} else {
			p_acc_gyro_raw_data = &acc_gyro_raw_data1[0];
		}

		break;

	case DMA_MAG:

		TODO: Look into how mag_raw_data is created in lsm303agr
		// Mag call back with current raw data
		pMagCallback(p_mag_raw_data, 1);

		// Swap raw data storage
		if (p_mag_raw_data == &mag_raw_data1) {
			p_mag_raw_data = &mag_raw_data2;
		} else {
			p_mag_raw_data = &mag_raw_data1;
		}

		break;

	default:
		SEGGER_SYSVIEW_PrintfHost("in_process_dma Error");
		assert_param(!HAL_OK);
		break;
	}

	in_process_dma = DMA_RDY; // Indicate ready for more

	// Schedule next pending
	run_pending_dma();
}





