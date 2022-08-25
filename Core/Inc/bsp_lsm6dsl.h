#ifndef BSP_LSM6DSL_H
#define BSP_LSM6DSL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "lsm6dsl.h"

#define GYRO_ACC_AVG_SAMPLES 10
#define GYRO_ACC_TOTAL_SAMPLES (GYRO_ACC_AVG_SAMPLES * 2)
#define GYRO_ACC_COMM_QUEUE_SIZE 3
#define GYRO_ACC_RATE 6 // 416Hz


int32_t bsp_lsm6dsl_init(void);
void bsp_lsm6dsl_enable(void);
void bsp_lsm6dsl_disable(void);


typedef struct {
	lsm6dsl_axis3bit32_t gyro_data;
	lsm6dsl_axis3bit32_t acc_data;
	uint32_t timeStamp;
} gyro_acc_rec_t;


#ifdef __cplusplus
}
#endif

#endif
