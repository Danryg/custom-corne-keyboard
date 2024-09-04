#! /bin/bash


echo Compiling script $1
arduino-cli compile --fqbn arduino:avr:micro $1

echo Uploading script $1
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:micro $1

