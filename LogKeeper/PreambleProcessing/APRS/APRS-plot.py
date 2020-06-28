# The purpose of this utility is to produce plot data (x, y, z) from a
# previously extracted aircraft stream. The input is stdin and the output
# is stdout. 
from ogn.parser import parse_aprs, ParseError
import sys
import os
import argparse

def main():
  parser = argparse.ArgumentParser(description = 'Generate plot data')
  parser.add_argument('--zmax', type = int, help = 'Max Z value')
  parser.add_argument('--zmin', type = int, help = 'Min Z value')
  args = parser.parse_args()

  for sin in sys.stdin:
    try:
      try:
        beacon = parse_aprs(sin)
      except ParseError as e:
        print('Error, {}'.format(e.message), file = sys.stderr)

      if beacon['aprs_type'] == 'position':
        try:
          if (beacon['altitude'] >= args.zmin) & (beacon['altitude'] <= args.zmax):
            print('{} {} {}'.format(beacon['latitude'],
                                    beacon['longitude'],
                                    beacon['altitude']))
        except KeyError:
          print('KeyError', file = sys.stderr)
    except StopIteration as e:
      print('StopIteration, {}'.format(e.message), file = sys.stderr)

if __name__ == "__main__":
  main()

