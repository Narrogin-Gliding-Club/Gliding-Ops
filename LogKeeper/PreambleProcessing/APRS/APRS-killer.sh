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

file_base=$(date "+%Y-%m-%d")
data_path=${BASE}/Data/${station}/APRS
for locker in $(ls ${data_path}/*.lock 2> /dev/null)
do
  kill -SIGINT $(cat ${locker})
  kill -SIGHUP $(cat ${locker})
  rm ${locker}
done
