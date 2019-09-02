################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/analogin_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/analogout_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/can_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/ethernet_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_irq_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/i2c_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/pinmap.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/port_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/pwmout_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/rtc_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/serial_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/sleep.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/spi_api.c \
../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/us_ticker.c 

OBJS += \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/analogin_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/analogout_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/can_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/ethernet_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_irq_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/i2c_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/pinmap.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/port_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/pwmout_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/rtc_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/serial_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/sleep.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/spi_api.o \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/us_ticker.o 

C_DEPS += \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/analogin_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/analogout_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/can_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/ethernet_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_irq_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/i2c_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/pinmap.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/port_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/pwmout_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/rtc_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/serial_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/sleep.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/spi_api.d \
./mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/us_ticker.d 


# Each subdirectory must supply rules for building sources it contributes
mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/%.o: ../mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_LPC1768 -DTARGET_M3 -DTARGET_NXP -DTARGET_LPC176X -DTOOLCHAIN_GCC_CR -DTOOLCHAIN_GCC -D__CORTEX_M3 -DARM_MATH_CM3 -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\hal" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\api" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal\TARGET_NXP" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC176X" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC176X" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC176X\TOOLCHAIN_GCC_CR" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\common" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\QEI" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


