################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/cmsis_nvic.c \
../mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/system_LPC17xx.c 

OBJS += \
./mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/cmsis_nvic.o \
./mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/system_LPC17xx.o 

C_DEPS += \
./mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/cmsis_nvic.d \
./mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/system_LPC17xx.d 


# Each subdirectory must supply rules for building sources it contributes
mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/%.o: ../mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_LPC1768 -DTARGET_M3 -DTARGET_NXP -DTARGET_LPC176X -DTOOLCHAIN_GCC_CR -DTOOLCHAIN_GCC -D__CORTEX_M3 -DARM_MATH_CM3 -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\hal" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\api" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal\TARGET_NXP" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC176X" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC176X" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC176X\TOOLCHAIN_GCC_CR" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\common" -I"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\QEI" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


