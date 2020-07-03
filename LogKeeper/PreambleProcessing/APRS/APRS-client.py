from ogn.client import AprsClient
from ogn.parser import parse_aprs, ParseError
import os
import sys
 
def process_beacon(raw_message):
  print(raw_message)
  sys.stdout.flush()
 
client = AprsClient(aprs_user = sys.argv[1], aprs_filter = sys.argv[2])
client.connect()

try:
  client.run(callback=process_beacon, autoreconnect = True)
except KeyboardInterrupt:
  print('\nStop ogn gateway', file = sys.stderr)
  client.disconnect()
