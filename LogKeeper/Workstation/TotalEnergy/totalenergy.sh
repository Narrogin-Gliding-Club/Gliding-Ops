#!/bin/bash

BASE=${HOME}/Work/NGC/Gliding-Ops/LogKeeper
EXECUTABLE_PATH=${BASE}/Workstation/TotalEnergy
LIB_BASE=${BASE}/lib
export PYTHONPATH=${PYTHONPATH}:${LIB_BASE}/python-ogn-client

zero="0"
while [ $# -gt 0 ]
do
  case ${1}
  in
    -z)
      shift
      zero=${1}
      ;;
    *)
      shift
      ;;
  esac
  shift
done

python3 ${EXECUTABLE_PATH}/totalenergy.py ${zero} $@
