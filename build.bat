@echo off

set BUILD_TYPE=Release
call %~dp0solution_generate.bat
if %errorlevel% NEQ 0 goto Error

set start_time=%DATE% %TIME%

echo building
cmake --build %BUILD_DIR% --config Release
if %errorlevel% NEQ 0 goto Error

:Error
echo someting went wrong :(
exit /B 1

:End
echo working time
echo -- Start: %start_time%
echo -- Stop:  %DATE% %TIME%
