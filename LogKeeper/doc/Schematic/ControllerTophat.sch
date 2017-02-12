v 20130925 2
C 40000 40000 0 0 0 title-B.sym
C 53900 50000 1 270 0 connector6-1.sym
{
T 55700 48200 5 10 0 0 270 0 1
device=CONNECTOR_6
T 55900 49900 5 10 1 1 270 0 1
refdes=P4
}
C 50800 50000 1 270 0 connector8-1.sym
{
T 54000 49900 5 10 0 0 270 0 1
device=CONNECTOR_8
T 53400 49900 5 10 1 1 270 0 1
refdes=P3
}
C 47500 50000 1 270 0 connector8-1.sym
{
T 50700 49900 5 10 0 0 270 0 1
device=CONNECTOR_8
T 50100 49900 5 10 1 1 270 0 1
refdes=P2
}
C 43400 50000 1 270 0 connector10-1.sym
{
T 46400 48100 5 10 0 0 270 0 1
device=CONNECTOR_10
T 46600 49900 5 10 1 1 270 0 1
refdes=P1
}
C 52800 46100 1 90 0 resistor-1.sym
{
T 52400 46400 5 10 0 0 90 0 1
device=RESISTOR
T 52400 46600 5 10 1 1 180 0 1
refdes=R1
T 52800 46500 5 10 1 1 0 0 1
value=2K
}
C 52800 45200 1 90 0 resistor-1.sym
{
T 52400 45500 5 10 0 0 90 0 1
device=RESISTOR
T 52400 45800 5 10 1 1 180 0 1
refdes=R2
T 53100 45800 5 10 1 1 180 0 1
value=1K
}
C 54000 46200 1 90 0 resistor-1.sym
{
T 53600 46500 5 10 0 0 90 0 1
device=RESISTOR
T 53500 46500 5 10 1 1 0 0 1
refdes=R3
T 54300 46600 5 10 1 1 180 0 1
value=4K
}
C 54000 45300 1 90 0 resistor-1.sym
{
T 53600 45600 5 10 0 0 90 0 1
device=RESISTOR
T 53500 45600 5 10 1 1 0 0 1
refdes=R4
T 54000 45600 5 10 1 1 0 0 1
value=1K
}
N 54400 46200 54400 48300 4
N 53200 46100 53200 47700 4
N 53200 47700 54100 47700 4
N 54100 47700 54100 48300 4
C 53800 44400 1 0 0 gnd-1.sym
{
T 54000 44400 5 10 1 1 0 0 1
refdes=A
}
N 52700 44700 53900 44700 4
N 43200 47400 52700 47400 4
N 43200 41700 52700 41700 4
C 44400 48000 1 0 0 gnd-1.sym
{
T 44600 48000 5 10 1 1 0 0 1
refdes=A
}
N 43200 44700 46300 44700 4
N 46300 44700 46300 48300 4
N 46000 45000 46000 48300 4
N 43200 45300 45700 45300 4
N 45400 45600 45400 48300 4
N 43200 45600 45400 45600 4
N 45700 45300 45700 48300 4
C 51500 47700 1 0 0 gnd-1.sym
N 51600 48000 51600 48300 4
N 51600 48300 51300 48300 4
N 51300 48300 51300 41600 4
N 51300 43800 43200 43800 4
N 51900 48300 52000 43200 4
N 43200 43200 52000 43200 4
N 48900 48300 48900 46800 4
N 43200 46800 48900 46800 4
N 49200 48300 49200 46500 4
N 43200 46500 49200 46500 4
N 51300 42300 43200 42300 4
C 41500 41500 1 0 0 connector20-1.sym
{
T 41600 47700 5 10 1 1 0 0 1
refdes=P5
T 42200 48000 5 10 0 0 0 0 1
device=CONNECTOR_20
T 41100 47400 5 10 1 1 0 0 1
pinlabel=B+
T 41100 41700 5 10 1 1 0 0 1
pinlabel=B-
T 41100 47100 5 10 1 1 0 0 1
pinlabel=P+
T 41100 42000 5 10 1 1 0 0 1
pinlabel=P-
T 41100 45600 5 10 1 1 0 0 1
pinlabel=K1
T 41100 45300 5 10 1 1 0 0 1
pinlabel=K2
T 41100 45000 5 10 1 1 0 0 1
pinlabel=K3
T 41100 44700 5 10 1 1 0 0 1
pinlabel=K4
T 41000 43800 5 10 1 1 0 0 1
pinlabel=GND
T 41000 43200 5 10 1 1 0 0 1
pinlabel=+5V
T 41100 46500 5 10 1 1 0 0 1
pinlabel=SCL
T 41100 46800 5 10 1 1 0 0 1
pinlabel=SDA
T 41000 42600 5 10 1 1 0 0 1
pinlabel=GND
T 41000 42300 5 10 1 1 0 0 1
pinlabel=GND
}
N 43200 42300 43200 42600 4
N 43700 46800 43700 46200 4
N 43700 46200 43200 46200 4
N 43900 46500 43900 45900 4
N 43900 45900 43200 45900 4
T 41100 46200 5 10 1 1 0 0 1
pinlabel=SDA
T 41100 45900 5 10 1 1 0 0 1
pinlabel=SCL
N 53900 46200 54400 46200 4
N 52700 46100 53200 46100 4
N 52700 47000 52700 47400 4
N 43200 47100 53900 47100 4
N 52700 41700 52700 45200 4
N 53900 45300 53900 44700 4
N 43200 45000 46000 45000 4
N 43200 42000 43500 42000 4
N 43500 42000 43500 41700 4
