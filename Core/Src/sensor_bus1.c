#include "sensor_bus1.h"
#include "main.h"
#include "stm32f7xx_hal.h"
#include "bsp_lsm6dsl.h"
#include "bsp_lsm303agr.h"
#include <lsm6dsl.h>
#include <stm32f7xx_nucleo_bus.h>
#include <stm32f7xx_hal_i2c.h>
#include <stdint.h>

#define DMA_QUEUE_SIZE 5

static volatile BUS1_DMA_TypeDef in_process_dma = DMA_RDY;
static volatile GP_StatusTypeDef req_mag = GP_FALSE;
static volatile GP_StatusTypeDef req_gyro_acc = GP_FALSE;
static volatile GP_StatusTypeDef lock_update = GP_FALSE;

static lsm6dsl_axis3bit16_t acc_gyro_raw_data[GYRO_ACC_TOTAL_SAMPLES];
static uint16_t acc_gyro_raw_data_size = sizeof(lsm6dsl_axis3bit16_t) * GYRO_ACC_TOTAL_SAMPLES;
static LSM303AGR_AxesRaw_t mag_value;

void run_pending_dma(void);
void schedule_next_dma(void);


void SensorBus1_Init(void) {

	req_mag = GP_FALSE;
	req_gyro_acc = GP_FALSE;
	lock_update = GP_FALSE;
	in_process_dma = DMA_RDY;

	bsp_lsm6dsl_init();
	bsp_lsm303agr_init();
}


void SensorBus1_Start(void) {
	bsp_lsm6dsl_enable();
	bsp_lsm303agr_enable();
}

void SensorBus1_Update(void) {

	// Set based on pin state, using this method help with missed updates when stopped for debug
	if (HAL_GPIO_ReadPin(MagDataRdy_GPIO_Port, MagDataRdy_Pin) == GPIO_PIN_SET) {
		req_mag = GP_TRUE;
	}


	if (HAL_GPIO_ReadPin(Lms6dsl_Int1_GPIO_Port, Lms6dsl_Int1_Pin) == GPIO_PIN_SET) {
		req_gyro_acc = GP_TRUE;
	}

	run_pending_dma();
}


void run_pending_dma(void) {

	// Must not be reentrant, this will catch that
	// Must have SensorBus1_DMA_CallBack and SensorBus1_Update on same interrupt level
	assert_param(lock_update == GP_FALSE);
	lock_update = GP_TRUE;

	if (in_process_dma != DMA_RDY) { // exit if previous still running
		lock_update = GP_FALSE;
		return;
	}

	// Check for mag DMA read
	if (req_mag) {
		assert_param(HAL_I2C_Mem_Read_DMA(&hi2c1, LSM303AGR_I2C_ADD_MG,
				LSM303AGR_OUTX_L_REG_M, I2C_MEMADD_SIZE_8BIT,
				(uint8_t*) &mag_value, sizeof(LSM303AGR_AxesRaw_t)) == HAL_OK);

		in_process_dma = DMA_MAG;
		req_mag = GP_FALSE;

	  // Else check for ryro/acc DMA read
	} else if (req_gyro_acc){
		assert_param(HAL_I2C_Mem_Read_DMA(&hi2c1, LSM6DSL_I2C_ADD_H, LSM6DSL_FIFO_DATA_OUT_L,
				I2C_MEMADD_SIZE_8BIT, (uint8_t*) &acc_gyro_raw_data,
				acc_gyro_raw_data_size) == HAL_OK);

		in_process_dma = DMA_ACC_GYRO;
		req_gyro_acc = GP_FALSE;
	}

	lock_update = GP_FALSE;
}


void SensorBus1_DMA_CallBack(void) {


	// Handle data ready from last dma request
	switch (in_process_dma) {

	case DMA_MAG:
		break;

	case DMA_ACC_GYRO:
		break;

	default:
		// should have something to do???
		break;
	}

	in_process_dma = DMA_RDY; // Indicate ready for more

	// Schedule next pending
	run_pending_dma();
}





