#!/bin/bash

pushd /home/pi/dump1090 > /dev/null
setsid ./dump1090 --net --quiet < /dev/null &> /dev/null > /dev/null & jobs -p %1 > ${HOME}/bin/dump1090.lock
popd > /dev/null
