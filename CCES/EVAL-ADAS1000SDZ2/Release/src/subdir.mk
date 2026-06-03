################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/EVAL-ADAS1000SDZ.c \
../src/ecg_leads.c \
../src/ecg_vector.c 

SRC_OBJS += \
./src/EVAL-ADAS1000SDZ.doj \
./src/ecg_leads.doj \
./src/ecg_vector.doj 

C_DEPS += \
./src/EVAL-ADAS1000SDZ.d \
./src/ecg_leads.d \
./src/ecg_vector.d 


# Each subdirectory must supply rules for building sources it contributes
src/EVAL-ADAS1000SDZ.doj: ../src/EVAL-ADAS1000SDZ.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/EVAL-ADAS1000SDZ.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_leads.doj: ../src/ecg_leads.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_leads.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_vector.doj: ../src/ecg_vector.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -DCORE0 -DNDEBUG -DNO_UTILITY_ROM_UART @includes-7057b62570194621fddff9c60d599c6a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_vector.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


