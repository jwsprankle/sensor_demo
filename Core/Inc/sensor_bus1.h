#ifndef SENSOR_BUS1_H
#define SENSOR_BUS1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "general_purpose_defs.h"

typedef enum {
	DMA_RDY = 0, DMA_MAG = 1, DMA_ACC_GYRO = 2
} BUS1_DMA_TypeDef;



extern void SensorBus1_Init(void);
extern void SensorBus1_Start(void);
extern void SensorBus1_Update(void);


#ifdef __cplusplus
}
#endif
#endif

