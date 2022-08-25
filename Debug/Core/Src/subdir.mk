################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bsp_lsm303agr.c \
../Core/Src/bsp_lsm6dsl.c \
../Core/Src/freertos.c \
../Core/Src/main.c \
../Core/Src/non_block_queue_ndx.c \
../Core/Src/sensor_bus1.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_hal_timebase_tim.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/stm32f7xx_nucleo_bus.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c 

OBJS += \
./Core/Src/bsp_lsm303agr.o \
./Core/Src/bsp_lsm6dsl.o \
./Core/Src/freertos.o \
./Core/Src/main.o \
./Core/Src/non_block_queue_ndx.o \
./Core/Src/sensor_bus1.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_hal_timebase_tim.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/stm32f7xx_nucleo_bus.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o 

C_DEPS += \
./Core/Src/bsp_lsm303agr.d \
./Core/Src/bsp_lsm6dsl.d \
./Core/Src/freertos.d \
./Core/Src/main.d \
./Core/Src/non_block_queue_ndx.d \
./Core/Src/sensor_bus1.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_hal_timebase_tim.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/stm32f7xx_nucleo_bus.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG=USE_HAL_DRIVER -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_ASSERT=1 '-D_packed=__attribute__((__packed__))' '-D_weak=__attribute__((weak))' -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/SEGGER -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/Components/lsm6dsl -I../Drivers/BSP/Components/lsm303agr -I../Drivers/BSP/Components/hts221 -I../Drivers/BSP/Components/lps22hb -I../Drivers/BSP/IKS01A2 -I../Drivers/BSP/Components/Common -I../Middlewares/ST/STM32_MotionMC_Library/Inc -I../Middlewares/ST/STM32_MotionEC_Library/Inc -I../Middlewares/ST/STM32_MotionFX_Library/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/bsp_lsm303agr.d ./Core/Src/bsp_lsm303agr.o ./Core/Src/bsp_lsm303agr.su ./Core/Src/bsp_lsm6dsl.d ./Core/Src/bsp_lsm6dsl.o ./Core/Src/bsp_lsm6dsl.su ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/non_block_queue_ndx.d ./Core/Src/non_block_queue_ndx.o ./Core/Src/non_block_queue_ndx.su ./Core/Src/sensor_bus1.d ./Core/Src/sensor_bus1.o ./Core/Src/sensor_bus1.su ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_hal_timebase_tim.d ./Core/Src/stm32f7xx_hal_timebase_tim.o ./Core/Src/stm32f7xx_hal_timebase_tim.su ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/stm32f7xx_nucleo_bus.d ./Core/Src/stm32f7xx_nucleo_bus.o ./Core/Src/stm32f7xx_nucleo_bus.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su

.PHONY: clean-Core-2f-Src

