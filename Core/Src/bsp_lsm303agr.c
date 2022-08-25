#include "bsp_lsm303agr.h"
#include "lsm303agr.h"
#include "stm32f7xx_nucleo_bus.h"
#include "lsm6dsl_reg.h"
#include "lsm303agr_reg.h"
#include "main.h"
#include "non_block_queue_ndx.h"

static LSM303AGR_MAG_Object_t bsp_mag_lsm303agr_Obj = { 0 };

int32_t bsp_lsm303agr_init() {
	LSM303AGR_IO_t mag_io_ctx;
	uint8_t id;

	mag_io_ctx.BusType = LSM303AGR_I2C_BUS;
	mag_io_ctx.Address = LSM303AGR_I2C_ADD_MG;
	mag_io_ctx.Init = BSP_I2C1_Init;
	mag_io_ctx.DeInit = BSP_I2C1_DeInit;
	mag_io_ctx.ReadReg = BSP_I2C1_ReadReg;
	mag_io_ctx.WriteReg = BSP_I2C1_WriteReg;
	mag_io_ctx.GetTick = BSP_GetTick;

	if (LSM303AGR_MAG_RegisterBusIO(&bsp_mag_lsm303agr_Obj,
			&mag_io_ctx) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	}

	// Vefify sensor ID (Just to verify that connection is make to chip)
	if (LSM303AGR_MAG_ReadID(&bsp_mag_lsm303agr_Obj, &id) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	}

	if (id != LSM303AGR_ID_MG) {
		return LSM303AGR_ERROR;
	}

	// Reset device and wait for powerup
	if (lsm303agr_mag_boot_set(&bsp_mag_lsm303agr_Obj.Ctx, 1) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	}
	HAL_Delay(100);

	// Vefify sensor ID (This is to just verify that connection is make to chip)
	if (LSM303AGR_MAG_ReadID(&bsp_mag_lsm303agr_Obj, &id) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	} else {
		if (id != LSM303AGR_ID_MG) {
			return LSM303AGR_ERROR;
		}
	}

	// Standard init
	if (LSM303AGR_MAG_Init(&bsp_mag_lsm303agr_Obj) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	}

	// Setup interrupt on data ready

	if (lsm303agr_mag_drdy_on_pin_set(&bsp_mag_lsm303agr_Obj.Ctx,
			1) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	}

	/* Output data rate selection */
//  if (lsm303agr_mag_data_rate_set(&(bsp_mag_lsm303agr_Obj.Ctx), LSM303AGR_MG_ODR_50Hz) != LSM303AGR_OK)
//  {
//    return LSM303AGR_ERROR;
//  }

	return LSM303AGR_OK;
}

int32_t bsp_lsm303agr_enable(void) {
	// Start up mag
	if (LSM303AGR_MAG_Enable(&bsp_mag_lsm303agr_Obj) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	}

	return LSM303AGR_OK;
}

int32_t bsp_lsm303agr_disable(void) {
	// Start up mag
	if (LSM303AGR_MAG_Disable(&bsp_mag_lsm303agr_Obj) != LSM303AGR_OK) {
		return LSM303AGR_ERROR;
	}

	return LSM303AGR_OK;
}

