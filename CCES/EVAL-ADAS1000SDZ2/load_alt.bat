@echo off
echo ========================================
echo Loading EVAL-ADAS1000SDZ via elfloader
echo ========================================
echo.

set CCES_PATH=D:\CCES2.11
set DXE_FILE=E:\CCES\EVAL-ADAS1000SDZ\Debug\EVAL-ADAS1000SDZ.dxe

echo DXE file: %DXE_FILE%
echo.

if not exist "%DXE_FILE%" (
    echo ERROR: DXE file not found!
    pause
    exit /b 1
)

echo Loading...
"%CCES_PATH%\cces\bin\elfloader.exe" -proc ADSP-BF527 -si-revision any -device SDP -driver "%CCES_PATH%\cces\bin\sdp_driver.dll" -load "%DXE_FILE%"

if %errorlevel% equ 0 (
    echo Load successful!
) else (
    echo Load failed with error code: %errorlevel%
)

pause