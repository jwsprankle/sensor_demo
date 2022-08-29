#ifndef SENSOR_BUS1_H
#define SENSOR_BUS1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "bsp_Lsm6dsl.h"
#include "bsp_lsm303agr.h"


// Note: add note about time limit on pRawData
typedef void (*AccGyroCallback_t)(GYRO_ACC_t * pRawData, uint16_t numItems);
typedef void (*MagCallback_t)(LSM303AGR_AxesRaw_t * pRawData, uint16_t numItems);


extern void InitSensorBus1(AccGyroCallback_t pAccGyroCB, MagCallback_t pMagCB);
extern void StartSensorBus1(void);
extern void UpdateSensorBus1(void);
extern void SensorBus1_DMA_CallBack(void);



#ifdef __cplusplus
}
#endif
#endif

