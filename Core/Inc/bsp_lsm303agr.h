#ifndef BSP_LSM303AGR_H
#define BSP_LSM303AGR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <lsm303agr.h>

#define MAG_ACC_AVG_SAMPLES 10
#define MAG_ACC_TOTAL_SAMPLES (MAG_ACC_AVG_SAMPLES * 2)
#define Mag_ACC_COMM_QUEUE_SIZE 3
#define Mag_ACC_RATE 6 // 416Hz


int32_t bsp_lsm303agr_init(void);
void bsp_lsm303agr_int_callback(void);
int32_t bsp_lsm303agr_enable(void);
int32_t bsp_lsm303agr_disable(void);


#ifdef __cplusplus
}
#endif

#endif
