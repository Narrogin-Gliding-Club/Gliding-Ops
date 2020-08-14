# The purpose of this test is to evaluate the use of total energy to detect
# aircraft takeoff and landing.

import sys
import os
import getopt
import datetime, time
import math

def main():
  airborne     = False
  takoff_time  = 0
  landing_time = 0
  last_time    = 0

  for sin in sys.stdin:
    parsed = sin.split(' ')
    time = int(parsed[0])
    if time < last_time:
      continue  # OGN sometimes spits out out of sequence data
    else:
      last_time = time
    energy = float(parsed[1])
    if airborne == False:
      if energy > 600:
        airborne = True
        print('Takeoff at {}'.format(time))
    elif airborne == True:
      if energy < 400:
        airborne = False
        print('Landing at {}'.format(time))
        sys.stdout.flush()

if __name__ == "__main__":
  main()
