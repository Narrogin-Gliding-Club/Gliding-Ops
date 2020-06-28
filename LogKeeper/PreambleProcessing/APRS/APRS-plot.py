# The purpose of this utility is to extract a specified aircraft from a
# APRS stream. The input is stdin and the output is stdout. The desired
# aircraft is specified by the six digit hex code.
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
            dt = beacon['timestamp']
            print('{} {} {}'.format(dt.hour * 3600 + dt.minute * 60 + dt.second,
                                    beacon['altitude'],

                                    beacon['ground_speed']))
          elif energy == False and velocity == True:
            c = beacon['comment']
            cv = c.split(' ')
            dt = beacon['timestamp']
            print('{} {} {}'.format(dt.hour * 3600 + dt.minute * 60 + dt.second,
                                    cv[1],
                                    beacon['ground_speed']))
          else:
            print("Logic error energy = {}, velocity = {}".format(energy,
                                                                  velocity))
        except KeyError:
          print('KeyError', file = sys.stderr)
    except StopIteration as e:
      print('StopIteration, {}'.format(e.message), file = sys.stderr)

def usage():
  print('Usage: {} [-e] [-v]'.format(sys.argv[1]))
  return

if __name__ == "__main__":
  main()

