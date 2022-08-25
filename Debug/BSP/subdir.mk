################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/Nucleo_F767ZI_GPIO.c \
../BSP/Nucleo_F767ZI_Init.c 

OBJS += \
./BSP/Nucleo_F767ZI_GPIO.o \
./BSP/Nucleo_F767ZI_Init.o 

C_DEPS += \
./BSP/Nucleo_F767ZI_GPIO.d \
./BSP/Nucleo_F767ZI_Init.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/%.o BSP/%.su: ../BSP/%.c BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx '-D_packed=__attribute__((__packed__))' '-D_weak=__attribute__((weak))' -DUSE_FULL_ASSERT=1 -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/SEGGER -I../BSP -I../Drivers/BSP/Components/lsm6dsl -I../Drivers/BSP/Components/lsm303agr -I../Drivers/BSP/Components/hts221 -I../Drivers/BSP/Components/lps22hb -I../X-CUBE-MEMS1/Target -I../Drivers/BSP/IKS01A2 -I../Drivers/BSP/Components/Common -I../Middlewares/ST/STM32_MotionMC_Library/Inc -I../Middlewares/ST/STM32_MotionEC_Library/Inc -I../Middlewares/ST/STM32_MotionFX_Library/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP

clean-BSP:
	-$(RM) ./BSP/Nucleo_F767ZI_GPIO.d ./BSP/Nucleo_F767ZI_GPIO.o ./BSP/Nucleo_F767ZI_GPIO.su ./BSP/Nucleo_F767ZI_Init.d ./BSP/Nucleo_F767ZI_Init.o ./BSP/Nucleo_F767ZI_Init.su

.PHONY: clean-BSP

