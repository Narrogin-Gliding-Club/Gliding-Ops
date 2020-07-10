#!/bin/bash

BASE=${HOME}/Work/NGC/Gliding-Ops/LogKeeper
EXECUTABLE_PATH=${BASE}/PreambleProcessing/APRS
LIB_BASE=${BASE}/lib
export PYTHONPATH=${PYTHONPATH}:${LIB_BASE}/python-ogn-client

file_base=$(date "+%Y-%m-%d")
DATA_BASE=${BASE}/Data
for station in $(ls -d *)
do
  if [ -d ${station} ]
  then
    data_path=${DATA_BASE}/${station}/APRS
    for locker in $(ls ${data_path}/*.lock 2> /dev/null)
    do
      kill -SIGINT $(cat ${locker})
      kill -SIGHUP $(cat ${locker})
      rm ${locker}
    done
  fi
done
