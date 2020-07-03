# The purpose of this utility is to produce a daily log file suitable for 
# algorithm evaluation and training.
from ogn.parser import parse_aprs, ParseError
import sys
import os
import getopt
import datetime, time

def main():
  line_count = 0;
  for sin in sys.stdin:
    try:
      try:
        beacon = parse_aprs(sin)
      except ParseError as e:
        print('Error, {}'.format(e.message), file = sys.stderr)

      if beacon['aprs_type'] == 'position':
        try:
          c = beacon['comment']
          cv = c.split(' ')
          dt = beacon['timestamp']
          print('{} {} {} {} {} {}'.format(dt.hour * 3600 + dt.minute * 60 + dt.second,
                                           beacon['latitude'],
                                           beacon['longitude'],
                                           beacon['altitude'],
                                           beacon['ground_speed'],
                                           cv[1]))

        except KeyError:
          print('KeyError', file = sys.stderr)
    except StopIteration as e:
      print('StopIteration, {}'.format(e.message), file = sys.stderr)

def usage():
  print('Usage: {} [-e] [-v] [-z <height>]'.format(sys.argv[1]))
  return

if __name__ == "__main__":
  main()

