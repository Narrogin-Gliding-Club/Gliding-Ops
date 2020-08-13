# The purpose of this test is to evaluate the use of total energy to detect
# aircraft takeoff and landing.
from ogn.parser import parse_aprs, ParseError
import sys
import os
import getopt
import datetime, time
import math

def main(zero):
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
      if record[1] != 'position':
        continue
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
  
    if record[1] == 'position':
      v = record[10] * 1000 / 3600  # Now in meters / second
      y = v * v * 0.5 + (record[11] - zero) * 9.8
      print('{} {} {} {}'.format(record[6].hour * 3600 +
                           record[6].minute * 60 +
                           record[6].second,
                           y,
                           record[11] - zero,
                           v))
      sys.stdout.flush()

if __name__ == "__main__":
  main(zero = float(sys.argv[1])) # No arg will give zero = 0.0

