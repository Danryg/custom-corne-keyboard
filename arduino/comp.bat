@echo off

REM Check arguments
if "%2" == "" (
    echo Usage: %0 port_name sketch_name
    exit /b 1
)

REM Set the sketch directory
set SKETCH_DIR=C:\Users\Danie\Documents\Github\projekt\keyboard\arduino\%2

REM Set the sketch file name (without extension)
set SKETCH_NAME=%2

REM Set the fully qualified board name (FQBN)
set BOARD_FQBN=arduino:avr:micro

REM Set the port
set PORT=%1

cd %SKETCH_DIR%

REM Compile the sketch
arduino-cli compile --fqbn %BOARD_FQBN% %SKETCH_NAME%.ino

REM Check if the compile step was successful
if %ERRORLEVEL% neq 0 (
    echo Compilation failed
    exit /b %ERRORLEVEL%
)

REM Upload the sketch
arduino-cli upload -p %PORT% --fqbn %BOARD_FQBN% %SKETCH_NAME%.ino

REM Check if the upload step was successful
if %ERRORLEVEL% neq 0 (
    echo Upload failed
    exit /b %ERRORLEVEL%
)

echo Upload successful
