#!/bin/bash

if [ $# -lt 1 ]
then
  echo ${0} "<filter from filter.txt> | <aircraft from aircraft.txt>"
  exit 255
fi

BASE=${HOME}/Work/NGC/Gliding-Ops/LogKeeper
EXECUTABLE_PATH=${BASE}/PreambleProcessing/APRS
LIB_BASE=${BASE}/lib
export PYTHONPATH=${PYTHONPATH}:${LIB_BASE}/python-ogn-client

hex=""
aircraft=""
grep -l ^${1} ${EXECUTABLE_PATH}/aircraft.txt > /dev/null
if [ $? -ne 0 ]
then
  hex=${1}
  aircraft='unknown'
  echo "Aircraft = $aircraft, hex = $hex" >&2
else
  aircraft=$(cat ${EXECUTABLE_PATH}/aircraft.txt | grep "^${1}" | cut -d ':' -f 1)
  hex=$(cat ${EXECUTABLE_PATH}/aircraft.txt | grep "^${1}" | cut -d ':' -f 2)
  echo "Aircraft = $aircraft, hex = $hex" >&2
fi
python3 ${EXECUTABLE_PATH}/APRS-extract.py ${aircraft} ${hex}
