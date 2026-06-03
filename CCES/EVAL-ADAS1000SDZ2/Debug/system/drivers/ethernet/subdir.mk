################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CCES2.11/Blackfin/lib/src/drivers/source/ethernet/common/adi_ether.c 

SRC_OBJS += \
./system/drivers/ethernet/adi_ether.doj 

C_DEPS += \
./system/drivers/ethernet/adi_ether.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/ethernet/adi_ether.doj: D:/CCES2.11/Blackfin/lib/src/drivers/source/ethernet/common/adi_ether.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/drivers/ethernet/adi_ether.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


