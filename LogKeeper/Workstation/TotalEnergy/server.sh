#!/bin/bash

BASE=${HOME}/Work/NGC/Gliding-Ops/LogKeeper
EXECUTABLE_PATH=${BASE}/Workstation/TotalEnergy
LIB_BASE=${BASE}/lib

python3 ${EXECUTABLE_PATH}/server.py $@
