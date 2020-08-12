#!/bin/bash

BASE=${HOME}/Work/NGC/Gliding-Ops/LogKeeper
EXECUTABLE_PATH=${BASE}/Workstation/TotalEnergy
LIB_BASE=${BASE}/lib
export PYTHONPATH=${PYTHONPATH}:${LIB_BASE}/python-ogn-client

python3 ${EXECUTABLE_PATH}/totalenergy.py $@
