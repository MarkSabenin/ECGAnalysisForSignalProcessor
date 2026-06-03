################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/ethernet/emac/adi_emac.c \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/ethernet/emac/adi_ether_phy.c 

SRC_OBJS += \
./system/drivers/ethernet/emac/adi_emac.doj \
./system/drivers/ethernet/emac/adi_ether_phy.doj 

C_DEPS += \
./system/drivers/ethernet/emac/adi_emac.d \
./system/drivers/ethernet/emac/adi_ether_phy.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/ethernet/emac/adi_emac.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/ethernet/emac/adi_emac.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/drivers/ethernet/emac/adi_emac.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/drivers/ethernet/emac/adi_ether_phy.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/ethernet/emac/adi_ether_phy.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/drivers/ethernet/emac/adi_ether_phy.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


