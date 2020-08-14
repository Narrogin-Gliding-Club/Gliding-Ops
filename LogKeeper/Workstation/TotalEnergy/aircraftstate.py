# The purpose of this test is to evaluate the use of total energy to detect
# aircraft takeoff and landing.

import sys
import os
import getopt
import datetime, time
import math

INIT    = 0
DORMANT = 1
ROLLING = 2
FLYING  = 3
LANDING = 4

height_error   = 0.0
height_error_n = 0
last_time      = 0

def init(t, e, h, v):
  global height_error, height_error_n
  height_error   = 0.0
  height_error_n = 0
  if v < 1:
    print('Dormant at {}'.format(t))
    return DORMANT
  return INIT

def dormant(t, e, h, v):
  global height_error, height_error_n, last_time
  if (t - last_time) > 120:  # Probably repowered
    return INIT
  if v > 1:
    if height_error_n > 0:
      height_error /= height_error_n
    print('Rolling at {} with QNH {}'.format(t, height_error))
    return ROLLING
  height_error   += h
  height_error_n += 1
  return DORMANT

def rolling(t, e, h, v):
  global height_error_n
  if v < 1:
    height_error_n = 1  # Count the current mean as 1 and continue
    print('Dormant at {}'.format(t))
    return DORMANT
  if e > 600:
    print('Takeoff at {}'.format(t))
    return FLYING
  return ROLLING

def flying(t, e, h, v):
  if e < 400:
    print('Landing at {}'.format(t))
    return LANDING
  return FLYING

def landing(t, e, h, v):
  global height_error, height_error_n
  if v < 1:
    height_error   = 0.0
    height_error_n = 0
    print('Dormant at {}'.format(t))
    return DORMANT
  return LANDING

states = { INIT:init,
           DORMANT:dormant,
           ROLLING:rolling,
           FLYING :flying,
           LANDING:landing }

def main():
  global last_time
  airborne     = False
  takoff_time  = 0
  landing_time = 0
  last_time    = 0
  state        = INIT

  for sin in sys.stdin:
    parsed = sin.split(' ')
    time = int(parsed[0])
    if time < last_time:
      continue  # OGN sometimes spits out out of sequence data
    height = float(parsed[2])
    vel    = float(parsed[3])
    energy = vel * vel * 0.5 + (height - height_error) * 9.8
    funct = states.get(state)
    state = funct(time, energy, height, vel)
    if (time > last_time):
      last_time = time
    sys.stdout.flush()

if __name__ == "__main__":
  main()
