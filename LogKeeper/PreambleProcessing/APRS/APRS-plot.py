# The purpose of this utility is to extract a specified aircraft from a
# APRS stream. The input is stdin and the output is stdout. The desired
# aircraft is specified by the six digit hex code.
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

      print('{} {} {}'.format(beacon['latitude'],
                              beacon['longitude'],
                              beacon['altitude']))

    except StopIteration:
      sin = ""

if __name__ == "__main__":
  main()
