################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/uart/adi_uart.c 

SRC_OBJS += \
./system/drivers/uart/adi_uart.doj 

C_DEPS += \
./system/drivers/uart/adi_uart.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/uart/adi_uart.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/uart/adi_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/drivers/uart/adi_uart.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


