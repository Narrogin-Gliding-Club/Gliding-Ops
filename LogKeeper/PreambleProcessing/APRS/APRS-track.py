# The purpose of this utility is to extract track data from an input stream.
# The stream should contain data only from one aircraft. This aircraft
# data can be selected with APRS-extract.
from ogn.parser import parse_aprs, ParseError
import sys
import os

def main():
  for sin in sys.stdin:
    try:
      try:
        beacon = parse_aprs(sin)
      except ParseError as e:
        print('Error, {}'.format(e.message), file = sys.stderr)

      if beacon['aprs_type'] == 'position':
        try:
          print('{},{},{},{},{},{}'.format(beacon['timestamp'],
                                           beacon['latitude'],
                                           beacon['longitude'],
                                           beacon['altitude'],
                                           beacon['ground_speed'],
                                           beacon['track']))
        except KeyError:
          print('KeyError', file = sys.stderr)
    except StopIteration as e:
      print('StopIteration, {}'.format(e.message), file = sys.stderr)

if __name__ == "__main__":
  main()

