# The purpose of this utility is to extract a specified aircraft from a
# APRS stream. The input is stdin and the output is stdout. The desired
# aircraft is specified by the six digit hex code.
from ogn.parser import parse_aprs, ParseError
import sys
import os
import getopt

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], "e")
  except getopt.GetoptError as err:
    print(err)
    usage()
    sys.exit(1)

  energy = False
  velocity = False
  for o, a in opts:
    if o == '-e':
      energy = True
    else:
      assert False

  for sin in sys.stdin:
    try:
      try:
        beacon = parse_aprs(sin)
      except ParseError as e:
        print('Error, {}'.format(e.message), file = sys.stderr)

      if beacon['aprs_type'] == 'position':
        try:
          if energy == False and velocity == False:
            print('{} {} {}'.format(beacon['latitude'],
                                    beacon['longitude'],
                                    beacon['altitude']))
          elif energy == True and velocity == False:
            print('{} {} {}'.format(beacon['timestamp'],
                                    beacon['altitude'],
                                    beacon['ground_speed']))
          else:
            print("Logic error energy = {}, velocity = {}".format(energy,
                                                                  velocity))
        except KeyError:
          print('KeyError', file = sys.stderr)
    except StopIteration as e:
      print('StopIteration, {}'.format(e.message), file = sys.stderr)

if __name__ == "__main__":
  main()

