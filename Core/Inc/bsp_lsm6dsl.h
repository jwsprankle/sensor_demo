#ifndef BSP_LSM6DSL_H
#define BSP_LSM6DSL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "lsm6dsl.h"

#define GYRO_ACC_SAMPLES 10
#define GYRO_ACC_COMM_QUEUE_SIZE 3
#define GYRO_ACC_RATE 6 // 416Hz


int32_t bsp_lsm6dsl_init(void);
void bsp_lsm6dsl_enable(void);
void bsp_lsm6dsl_disable(void);


typedef struct {
	LSM6DSL_AxesRaw_t gyro_data;
	LSM6DSL_AxesRaw_t acc_data;
	uint32_t timeStamp;
} GYRO_ACC_t;


#ifdef __cplusplus
}
#endif

#endif
