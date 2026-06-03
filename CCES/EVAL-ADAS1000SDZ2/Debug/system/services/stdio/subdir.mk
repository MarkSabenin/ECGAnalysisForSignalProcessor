################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio.c \
D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio_jtag.c \
D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio_null.c \
D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio_uart.c 

SRC_OBJS += \
./system/services/stdio/adi_stdio.doj \
./system/services/stdio/adi_stdio_jtag.doj \
./system/services/stdio/adi_stdio_null.doj \
./system/services/stdio/adi_stdio_uart.doj 

C_DEPS += \
./system/services/stdio/adi_stdio.d \
./system/services/stdio/adi_stdio_jtag.d \
./system/services/stdio/adi_stdio_null.d \
./system/services/stdio/adi_stdio_uart.d 


# Each subdirectory must supply rules for building sources it contributes
system/services/stdio/adi_stdio.doj: D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/services/stdio/adi_stdio_jtag.doj: D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio_jtag.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio_jtag.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/services/stdio/adi_stdio_null.doj: D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio_null.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio_null.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/services/stdio/adi_stdio_uart.doj: D:/CCES2.11/Blackfin/lib/src/services/source/stdio/adi_stdio_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio_uart.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


