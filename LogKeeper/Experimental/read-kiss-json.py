import json
import sys

r = json.load(sys.stdin)

aircraft = r['targets']
n = len(aircraft)

# For each aircraft output id time age lat lon alt speed course vertical  
for i in range(n):
  print(aircraft[i][0], aircraft[i][2],  aircraft[i][3], aircraft[i][4],  aircraft[i][5], aircraft[i][6], aircraft[i][9], aircraft[i][10], aircraft[i][11])
