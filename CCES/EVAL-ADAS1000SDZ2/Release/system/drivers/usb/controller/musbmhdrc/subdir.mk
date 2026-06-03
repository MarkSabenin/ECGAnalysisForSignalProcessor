################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/usb/controller/musbmhdrc/adi_usbd_musbmhdrc.c \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/usb/controller/musbmhdrc/adi_usbh_musbmhdrc.c 

SRC_OBJS += \
./system/drivers/usb/controller/musbmhdrc/adi_usbd_musbmhdrc.doj \
./system/drivers/usb/controller/musbmhdrc/adi_usbh_musbmhdrc.doj 

C_DEPS += \
./system/drivers/usb/controller/musbmhdrc/adi_usbd_musbmhdrc.d \
./system/drivers/usb/controller/musbmhdrc/adi_usbh_musbmhdrc.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/usb/controller/musbmhdrc/adi_usbd_musbmhdrc.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/usb/controller/musbmhdrc/adi_usbd_musbmhdrc.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/drivers/usb/controller/musbmhdrc/adi_usbd_musbmhdrc.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/drivers/usb/controller/musbmhdrc/adi_usbh_musbmhdrc.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.12.1/Blackfin/lib/src/drivers/source/usb/controller/musbmhdrc/adi_usbh_musbmhdrc.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/drivers/usb/controller/musbmhdrc/adi_usbh_musbmhdrc.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


