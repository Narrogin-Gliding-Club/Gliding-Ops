#!/bin/bash

EXECUTABLE_PATH=${HOME}/Work/Narrogin-Gliding-Club/Gliding-Ops/LogKeeper/Experimental/APRS

export PYTHONPATH=${PYTHONPATH}:${EXECUTABLE_PATH}/python-ogn-client
python3 ${EXECUTABLE_PATH}/APRS-client.py
