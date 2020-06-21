from ogn.client import AprsClient
from ogn.parser import parse_aprs, ParseError
 
def process_beacon(raw_message):
    if raw_message[0] == '#':
        print('Server Status: {}'.format(raw_message))
        return
 
    try:
        beacon = parse_aprs(raw_message)
        print('Received {aprs_type}: {raw_message}'.format(**beacon))
    except ParseError as e:
        print('Error, {}'.format(e.message))
 
client = AprsClient(aprs_user='YNRG', aprs_filter='r/-32.9276/117.0826/10')
#client = AprsClient(aprs_user='YBLA', aprs_filter='r/-36.5518/146.0057/10')
#client = AprsClient(aprs_user='YKRY', aprs_filter='r/-26.5836/151.8494/10')
#client = AprsClient(aprs_user='EDLM', aprs_filter='r/51.6473/7.1642/10')
#client = AprsClient(aprs_user='YNRG')
client.connect()

try:
    client.run(callback=process_beacon, autoreconnect=True)
except KeyboardInterrupt:
    print('\nStop ogn gateway')
    client.disconnect()
