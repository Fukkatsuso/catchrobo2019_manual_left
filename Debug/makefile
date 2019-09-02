################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include module/subdir.mk
-include mbed-src/targets/hal/TARGET_NXP/TARGET_LPC176X/subdir.mk
-include mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/TOOLCHAIN_GCC_CR/subdir.mk
-include mbed-src/targets/cmsis/TARGET_NXP/TARGET_LPC176X/subdir.mk
-include mbed-src/common/subdir.mk
-include QEI/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: manualcatcher_left.axf

# Tool invocations
manualcatcher_left.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\hal" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\api" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal\TARGET_NXP" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC176X" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC176X" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC176X\TOOLCHAIN_GCC_CR" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\common" -L"C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\QEI" -Xlinker -Map="manualcatcher_left.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "C:\Users\mutsuro\Documents\LPCXpresso_8.2.2_650\workspace\CatchRobo_2019\manualcatcher_left\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC176X\TOOLCHAIN_GCC_CR\LPC1768.ld" -o "manualcatcher_left.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) manualcatcher_left.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "manualcatcher_left.axf"; arm-none-eabi-objcopy -O binary "manualcatcher_left.axf" "manualcatcher_left.bin" ; # checksum -p LPC1768 -d "manualcatcher_left.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
