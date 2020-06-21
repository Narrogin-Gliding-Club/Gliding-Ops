from ogn.client import AprsClient
from ogn.parser import parse_aprs, ParseError
import os
import sys
 
def process_beacon(raw_message):
    if raw_message[0] == '#':
        print('Server Status: {}'.format(raw_message))
        return
 
    try:
        beacon = parse_aprs(raw_message)
        print('Received {aprs_type}: {raw_message}'.format(**beacon))
    except ParseError as e:
        print('Error, {}'.format(e.message))
 
client = AprsClient(aprs_user=sys.argv[1], aprs_filter=sys.argv[2])
client.connect()

try:
    client.run(callback=process_beacon, autoreconnect=True)
except KeyboardInterrupt:
    print('\nStop ogn gateway')
    client.disconnect()
