#!/bin/bash

source esp-idf/export.sh

export FIRMWARE_PATH="$(pwd)/app"
export PATH="$PATH:$(pwd)"
export PORT="/dev/ttyACM0"
