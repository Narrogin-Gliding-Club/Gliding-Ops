#!/bin/bash

if [ $# -lt 1 ]
then
  echo ${0} "<filter from filter.txt>"
  exit 255
fi

BASE=${HOME}/Work/NGC/Gliding-Ops/LogKeeper
EXECUTABLE_PATH=${BASE}/PreambleProcessing/APRS
LIB_BASE=${BASE}/lib
export PYTHONPATH=${PYTHONPATH}:${LIB_BASE}/python-ogn-client

grep -l ${1} ${EXECUTABLE_PATH}/filters.txt > /dev/null
if [ $? -ne 0 ]
then
  echo "${0}: Filter ${1} not found"
  exit 1
fi
station=$(cat ${EXECUTABLE_PATH}/filters.txt | grep "^${1}" | cut -d ':' -f 1)
filter=$(cat ${EXECUTABLE_PATH}/filters.txt | grep "^${1}" | cut -d ':' -f 2)
#echo "python3 ${EXECUTABLE_PATH}/APRS-client.py ${station} $filter"
echo "Station = ${station}, filter = $filter" >&2
python3 ${EXECUTABLE_PATH}/APRS-dict.py ${station} $filter
