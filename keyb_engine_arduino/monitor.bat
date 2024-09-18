@echo off

REM Check arguments
if "%1" == "" (
    echo Usage: %0 port_name
    exit /b 1
)


REM Set the sketch directory
set SKETCH_DIR=C:\Users\Danie\Documents\Github\projekt\keyboard\slave

REM Set the sketch file name (without extension)
REM set SKETCH_NAME=slave

REM Set the fully qualified board name (FQBN)
REM set BOARD_FQBN=arduino:avr:micro

REM Set the port
REM set PORT=%1

REM monitor the serial port
arduino-cli monitor -p %1

REM Check if the monitor step was successful
if %ERRORLEVEL% neq 0 (
    echo Monitor failed
    exit /b %ERRORLEVEL%
)

echo Monitor successful

