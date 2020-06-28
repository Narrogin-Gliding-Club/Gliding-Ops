# The purpose of this utility is to produce a plot of various parameters
# from a raw APRS stream. The stream is typically derived from one aircraft
# by APRS-extract.
from ogn.parser import parse_aprs, ParseError
import sys
import os
import getopt
import datetime, time

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], "ev")
  except getopt.GetoptError as err:
    print(err)
    usage()
    sys.exit(1)

  energy = False
  velocity =False 
  for o, a in opts:
    if o in ('-e'):
      energy = True
    elif o in ('-v'):
      velocity = True
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
  
  for record in records:
    if record[1] == 'position':
      print('{} {}'.format(record[6].hour * 3600 +
                           record[6].minute * 60 +
                           record[6].second,
                           record[10]))
  print()

  for record in records:
    if record[1] == 'position':
      print('{} {}'.format(record[6].hour * 3600 +
                           record[6].minute * 60 +
                           record[6].second,
                           record[11]))
  print()

  for record in records:
    if record[1] == 'position':
      print('{} {}'.format(record[6].hour * 3600 +
                           record[6].minute * 60 +
                           record[6].second,
                           record[10] * record[10] + record[11]))
  print()

def usage():
  print('Usage: {} [-e] [-v]'.format(sys.argv[1]))
  return

if __name__ == "__main__":
  main()

