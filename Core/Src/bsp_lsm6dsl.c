#include "sensor_bus1.h"
#include "main.h"
#include "bsp_Lsm6dsl.h"
#include "stm32f7xx_nucleo_bus.h"
#include "non_block_queue_ndx.h"
#include <lsm6dsl.h>
#include <lsm6dsl_reg.h>

static LSM6DSL_Object_t bsp_lsm6dsl_Obj = { 0 };

/**
 * @brief  Initialize the LSM6DSL sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */

int32_t bsp_lsm6dsl_init() {
	LSM6DSL_IO_t io_ctx;
	lsm6dsl_ctrl3_c_t ctrl3_c;
	uint8_t id;

	io_ctx.BusType = LSM6DSL_I2C_BUS;
	io_ctx.Address = LSM6DSL_I2C_ADD_H;
	io_ctx.Init = BSP_I2C1_Init;
	io_ctx.DeInit = BSP_I2C1_DeInit;
	io_ctx.ReadReg = BSP_I2C1_ReadReg;
	io_ctx.WriteReg = BSP_I2C1_WriteReg;
	io_ctx.GetTick = BSP_GetTick;

	if (LSM6DSL_RegisterBusIO(&bsp_lsm6dsl_Obj, &io_ctx) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	// Vefify sensor ID
	if (LSM6DSL_ReadID(&bsp_lsm6dsl_Obj, &id) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	if (id != LSM6DSL_ID) {
		return LSM6DSL_ERROR;
	}

	// Reset device
	if (lsm6dsl_reset_set(&bsp_lsm6dsl_Obj.Ctx, 1) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	// Wait for reset to complete
	do {
		if (lsm6dsl_read_reg(&bsp_lsm6dsl_Obj.Ctx, LSM6DSL_CTRL3_C,
				(uint8_t*) &ctrl3_c, 1) != LSM6DSL_OK) {
			return LSM6DSL_ERROR;
		}
	} while (ctrl3_c.sw_reset == 1);

	// Init using default settings
	if (LSM6DSL_Init(&bsp_lsm6dsl_Obj) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	// Override default data rate for acc and gyro
	bsp_lsm6dsl_Obj.acc_odr = (lsm6dsl_odr_xl_t) GYRO_ACC_RATE;
	bsp_lsm6dsl_Obj.gyro_odr = (lsm6dsl_odr_g_t) GYRO_ACC_RATE;

	LSM6DSL_ACC_Disable(&bsp_lsm6dsl_Obj);
	LSM6DSL_GYRO_Disable(&bsp_lsm6dsl_Obj);

	if (lsm6dsl_fifo_data_rate_set(&(bsp_lsm6dsl_Obj.Ctx),
			(lsm6dsl_odr_fifo_t) GYRO_ACC_RATE) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	if (LSM6DSL_FIFO_ACC_Set_Decimation(&bsp_lsm6dsl_Obj,
			LSM6DSL_FIFO_XL_NO_DEC) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	if (LSM6DSL_FIFO_GYRO_Set_Decimation(&bsp_lsm6dsl_Obj,
			LSM6DSL_FIFO_GY_NO_DEC) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	if (LSM6DSL_FIFO_Set_Mode(&bsp_lsm6dsl_Obj,
			LSM6DSL_STREAM_MODE) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	if (LSM6DSL_FIFO_Set_Watermark_Level(&bsp_lsm6dsl_Obj,
			GYRO_ACC_WATERMARK) != LSM6DSL_OK) {
		return LSM6DSL_ERROR;
	}

	// Setup watermark interrupt
	lsm6dsl_int1_route_t fth_route = { 0 };
	fth_route.int1_fth = 1;
	lsm6dsl_pin_int1_route_set(&(bsp_lsm6dsl_Obj.Ctx), fth_route);

	bsp_lsm6dsl_Obj.is_initialized = 1;

	// Enable interrupt
//	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	return LSM6DSL_OK;
}

//void bsp_lsm6dsl_request_acc_gyro(void)
//{

//	DebugExtAnalyzerSet1();
//	

//	// Read INT_1 pin state to determine if data is ready, this is faster than LSM6DSL_FIFO_Get_Full_Status which calls readi2c
//	// For now we assume only watermark interrupt is connected to int_1
//	
////	FIX ME, always SET!!!!! WHY!!!!!
////	
//////	if (HAL_GPIO_ReadPin(Lms6dsl_Int1_GPIO_Port, Lms6dsl_Int1_Pin) == GPIO_PIN_SET)
////	{
////		if (HAL_I2C_Mem_Read_DMA(&hi2c1, LSM6DSL_I2C_ADD_H, LSM6DSL_FIFO_DATA_OUT_L, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&rawDataSample, data_raw_size) != HAL_ERROR) 		
////		{   
////			// Signal that this is a acc_gyro_dma
////			request_read_acc_gyro_dma();
////		}
////		else
////		{
////			__NOP;
////		}
////	}
//	

//	DebugExtAnalyzerReset1();
//}

void bsp_lsm6dsl_enable(void) {
	LSM6DSL_ACC_Enable(&bsp_lsm6dsl_Obj);
	LSM6DSL_GYRO_Enable(&bsp_lsm6dsl_Obj);
}

void bsp_lsm6dsl_disable(void) {
	LSM6DSL_ACC_Disable(&bsp_lsm6dsl_Obj);
	LSM6DSL_GYRO_Disable(&bsp_lsm6dsl_Obj);
}

