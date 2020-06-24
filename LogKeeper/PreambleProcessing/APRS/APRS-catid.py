# The purpose of this utility is to extract all the position names from
# an APRS stream. Each instance of a position name is printed. Therefore,
# it is expected that a particular beacon name will be repeated. Use with
# sort(1) and uniq(1) to identify which beacons appear in a particular 
# stream.
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
        print('{}'.format(beacon['name']))

    except StopIteration:
      sin = ""

if __name__ == "__main__":
  main()

