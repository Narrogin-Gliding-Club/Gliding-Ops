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
    opts, args = getopt.getopt(sys.argv[1:], "evz", ['zero='])
  except getopt.GetoptError as err:
    print(err)
    usage()
    sys.exit(1)

  energy = False
  velocity =False 
  zero_height = 0.0
  for o, a in opts:
    if o in ('-e'):
      energy = True
    elif o in ('-v'):
      velocity = True
    elif o in ('-z', '--zero'):
      zero_height = float(a)
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
            vs = beacon['ground_speed'] * 1000 / 3600 # Now in meters / sec.
            dt = beacon['timestamp']
            print('{} {} {}'.format(dt.hour * 3600 + dt.minute * 60 + dt.second,
                                    (beacon['altitude'] - zero_height) * 9.8,
                                    vs * vs * 0.5))

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
  print('Usage: {} [-e] [-v] [-z <height>]'.format(sys.argv[1]))
  return

if __name__ == "__main__":
  main()

