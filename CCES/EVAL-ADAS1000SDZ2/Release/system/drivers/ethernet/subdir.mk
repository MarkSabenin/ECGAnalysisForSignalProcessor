################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/ethernet/common/adi_ether.c 

SRC_OBJS += \
./system/drivers/ethernet/adi_ether.doj 

C_DEPS += \
./system/drivers/ethernet/adi_ether.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/ethernet/adi_ether.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/ethernet/common/adi_ether.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/drivers/ethernet/adi_ether.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


