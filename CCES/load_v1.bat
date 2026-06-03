@echo off
echo ========================================
echo Loading EVAL-ADAS1000SDZ via cces_runner
echo ========================================

set CCES_PATH=D:\CCES2.11
set DXE_FILE=E:\CCES\EVAL-ADAS1000SDZ\Debug\EVAL-ADAS1000SDZ.dxe

echo DXE file: %DXE_FILE%
echo.

if not exist "%DXE_FILE%" (
    echo ERROR: DXE file not found!
    echo Please build the project first.
    pause
    exit /b 1
)

echo Loading program to target...
"%CCES_PATH%\cces\bin\cces_runner.exe" --core "0,%DXE_FILE%" --target "ADSP-BF527 via Debug Agent"

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo Load successful!
    echo ========================================
) else (
    echo.
    echo ========================================
    echo Load failed with error code: %errorlevel%
    echo ========================================
)

pause