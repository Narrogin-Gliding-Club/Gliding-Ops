#!/bin/bash

BASE=${HOME}/Work/NGC/Gliding-Ops/LogKeeper
EXECUTABLE_PATH=${BASE}/PreambleProcessing/APRS
LIB_BASE=${BASE}/lib
export PYTHONPATH=${PYTHONPATH}:${LIB_BASE}/python-ogn-client

file_base=$(date "+%Y-%m-%d")
DATA_PATH=${BASE}/Data/YNRG/APRS

for locker in $(ls ${DATA_PATH}/*.lock)
do
  kill -SIGHUP $(cat ${locker})
  rm ${locker}
done
