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
echo "Station = ${station}, filter = $filter" >&2
file_base=$(date "+%Y-%m-%d")
DATA_PATH=${BASE}/Data/YNRG/APRS
OUTPUT_FILE=${DATA_PATH}/${file_base}.raw.aprs
LOCK_FILE=${DATA_PATH}/${file_base}.raw.lock
echo "Writing to ${OUTPUT_FILE}"
if [ -f ${OUTPUT_FILE} ]
then
  python3 ${EXECUTABLE_PATH}/APRS-client.py ${station} $filter >> ${OUTPUT_FILE} &
else
  python3 ${EXECUTABLE_PATH}/APRS-client.py ${station} $filter > ${OUTPUT_FILE} &
fi
echo $! > ${LOCK_FILE}
