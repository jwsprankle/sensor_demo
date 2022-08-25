#ifndef ACC_GYRO_SENSOR_H
#define ACC_GYRO_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ACC_GYRO_SENSOR_OK 0
#define ACC_GYRO_SENSOR_ERROR -1

// Public functions
int32_t init_acc_gyro_sensor(void);


#ifdef __cplusplus
}
#endif

#endif /* ACC_GYRO_SENSOR_H */
