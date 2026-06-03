@echo off
echo Loading with elfloader...
D:\CCES2.11\elfloader.exe -proc ADSP-BF527 -si-revision any -driver "D:\CCES2.11\sdp_driver.dxe" -cmd prog -erase affected -format hex -file "E:\CCES\EVAL-ADAS1000SDZ\Debug\EVAL-ADAS1000SDZ.dxe"
pause