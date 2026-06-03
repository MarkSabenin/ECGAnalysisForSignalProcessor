################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/EVAL-ADAS1000SDZ.c \
../src/ecg_leads.c \
../src/ecg_predict.c \
../src/ecg_predict_data.c \
../src/ecg_predict_initialize.c \
../src/ecg_predict_terminate.c \
../src/ecg_vector.c \
../src/minOrMax.c \
../src/rtGetInf.c \
../src/rtGetNaN.c \
../src/rt_nonfinite.c 

SRC_OBJS += \
./src/EVAL-ADAS1000SDZ.doj \
./src/ecg_leads.doj \
./src/ecg_predict.doj \
./src/ecg_predict_data.doj \
./src/ecg_predict_initialize.doj \
./src/ecg_predict_terminate.doj \
./src/ecg_vector.doj \
./src/minOrMax.doj \
./src/rtGetInf.doj \
./src/rtGetNaN.doj \
./src/rt_nonfinite.doj 

C_DEPS += \
./src/EVAL-ADAS1000SDZ.d \
./src/ecg_leads.d \
./src/ecg_predict.d \
./src/ecg_predict_data.d \
./src/ecg_predict_initialize.d \
./src/ecg_predict_terminate.d \
./src/ecg_vector.d \
./src/minOrMax.d \
./src/rtGetInf.d \
./src/rtGetNaN.d \
./src/rt_nonfinite.d 


# Each subdirectory must supply rules for building sources it contributes
src/EVAL-ADAS1000SDZ.doj: ../src/EVAL-ADAS1000SDZ.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/EVAL-ADAS1000SDZ.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_leads.doj: ../src/ecg_leads.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_leads.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_predict.doj: ../src/ecg_predict.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_predict.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_predict_data.doj: ../src/ecg_predict_data.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_predict_data.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_predict_initialize.doj: ../src/ecg_predict_initialize.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_predict_initialize.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_predict_terminate.doj: ../src/ecg_predict_terminate.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_predict_terminate.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ecg_vector.doj: ../src/ecg_vector.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/ecg_vector.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/minOrMax.doj: ../src/minOrMax.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/minOrMax.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/rtGetInf.doj: ../src/rtGetInf.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/rtGetInf.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/rtGetNaN.doj: ../src/rtGetNaN.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/rtGetNaN.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/rt_nonfinite.doj: ../src/rt_nonfinite.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn -c -file-attr ProjectName="EVAL-ADAS1000SDZ" -proc ADSP-BF527 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG -DNO_UTILITY_ROM_UART @includes-88e0ff709f9b9f0091d579c594fdb0db.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -gnu-style-dependencies -MD -Mo "src/rt_nonfinite.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


