#!/bin/bash

cd "$FIRMWARE_PATH"
idf.py build && idf.py size
