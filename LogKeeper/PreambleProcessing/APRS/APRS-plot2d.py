# The purpose of this utility is to produce a plot of various parameters
# from a raw APRS stream. The stream is typically derived from one aircraft
# by APRS-extract.
from ogn.parser import parse_aprs, ParseError
import sys
import os
import getopt
import datetime, time
import math

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], "zvae", ["all", "zero="])
  except getopt.GetoptError as err:
    print(err)
    usage()
    sys.exit(1)

  everything = False
  altitude   = False
  velocity   = False
  energy     = False
  zero       = 0.0
  for o, a in opts:
    if o  in ('-z', '--zero'):
      zero = float(a)
    elif o == '-v':
      velocity = True
    elif o == '-a':
      altitude = True
    elif o == '-e':
      energy = True
    elif o == '--all':
      everything = True
    else:
      assert False

  records = []
  for sin in sys.stdin:
    try:
      try:
        beacon = parse_aprs(sin)
      except ParseError as e:
        print('Error, {}'.format(e.message), file = sys.stderr)

      record = []
      record.append(beacon['reference_timestamp'])
      record.append(beacon['aprs_type'])
      record.append(beacon['name'])
      record.append(beacon['dstcall'])
      record.append(beacon['relay'])
      record.append(beacon['receiver_name'])
      record.append(beacon['timestamp'])
      record.append(beacon['latitude'])
      record.append(beacon['longitude'])
      record.append(beacon['track'])
      record.append(beacon['ground_speed'])
      record.append(beacon['altitude'])
      record.append(beacon['comment'])
    except StopIteration as e:
      print('StopIteration, {}'.format(e.message), file = sys.stderr)
    records.append(record)
  
  if velocity == True or everything == True:
    for record in records:
      if record[1] == 'position':
        y = record[10]
        print('{} {}'.format(record[6].hour * 3600 +
                             record[6].minute * 60 +
                             record[6].second,
                             y))
    print()

  if altitude == True or everything == True:
    for record in records:
      if record[1] == 'position':
        y = record[11]
        print('{} {}'.format(record[6].hour * 3600 +
                             record[6].minute * 60 +
                             record[6].second,
                             y))
    print()

  if energy == True or everything == True:
    for record in records:
      if record[1] == 'position':
        v = record[10] * 1000 / 3600  # Now in meters / second
        y = v * v * 0.5 + (record[11] - zero) * 9.8
        print('{} {}'.format(record[6].hour * 3600 +
                             record[6].minute * 60 +
                             record[6].second,
                             y))
    print()

def usage():
  print('Usage: {} [-e] [-v] [-a] [-l] [--all]'.format(sys.argv[1]))
  return

if __name__ == "__main__":
  main()

