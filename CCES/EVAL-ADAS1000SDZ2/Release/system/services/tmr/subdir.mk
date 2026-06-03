################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/services/source/tmr/adi_ctmr.c \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/services/source/tmr/adi_tmr.c 

SRC_OBJS += \
./system/services/tmr/adi_ctmr.doj \
./system/services/tmr/adi_tmr.doj 

C_DEPS += \
./system/services/tmr/adi_ctmr.d \
./system/services/tmr/adi_tmr.d 


# Each subdirectory must supply rules for building sources it contributes
system/services/tmr/adi_ctmr.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/services/source/tmr/adi_ctmr.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/services/tmr/adi_ctmr.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/services/tmr/adi_tmr.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/services/source/tmr/adi_tmr.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/services/tmr/adi_tmr.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


