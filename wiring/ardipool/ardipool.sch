EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date "mar. 31 mars 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 10700 1000 0    60   ~ 0
Holes
$Comp
L Connector_Generic:Conn_01x01 P8
U 1 1 56D70B71
P 10600 650
F 0 "P8" V 10700 650 31  0000 C CNN
F 1 "CONN_01X01" V 10700 650 50  0001 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 10600 650 50  0001 C CNN
F 3 "" H 10600 650 50  0000 C CNN
	1    10600 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P9
U 1 1 56D70C9B
P 10700 650
F 0 "P9" V 10800 650 31  0000 C CNN
F 1 "CONN_01X01" V 10800 650 50  0001 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 10700 650 50  0001 C CNN
F 3 "" H 10700 650 50  0000 C CNN
	1    10700 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P10
U 1 1 56D70CE6
P 10800 650
F 0 "P10" V 10900 650 31  0000 C CNN
F 1 "CONN_01X01" V 10900 650 50  0001 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 10800 650 50  0001 C CNN
F 3 "" H 10800 650 50  0000 C CNN
	1    10800 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P11
U 1 1 56D70D2C
P 10900 650
F 0 "P11" V 11000 650 31  0000 C CNN
F 1 "CONN_01X01" V 11000 650 50  0001 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 10900 650 50  0001 C CNN
F 3 "" H 10900 650 50  0000 C CNN
	1    10900 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P12
U 1 1 56D711A2
P 11000 650
F 0 "P12" V 11100 650 31  0000 C CNN
F 1 "CONN_01X01" V 11100 650 50  0001 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 11000 650 50  0001 C CNN
F 3 "" H 11000 650 50  0000 C CNN
	1    11000 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P13
U 1 1 56D711F0
P 11100 650
F 0 "P13" V 11200 650 31  0000 C CNN
F 1 "CONN_01X01" V 11200 650 50  0001 C CNN
F 2 "Socket_Arduino_Mega:Arduino_1pin" H 11100 650 50  0001 C CNN
F 3 "" H 11100 650 50  0000 C CNN
	1    11100 650 
	0    -1   -1   0   
$EndComp
NoConn ~ 10600 850 
NoConn ~ 10700 850 
NoConn ~ 10800 850 
NoConn ~ 10900 850 
NoConn ~ 11000 850 
NoConn ~ 11100 850 
Wire Notes Line
	10450 1000 10450 500 
Wire Notes Line
	11200 1000 10450 1000
Wire Notes Line
	11600 6500 11600 950 
$Comp
L Sensor_Temperature:DS18B20 U1
U 1 1 5EAE9D8E
P 1150 4000
F 0 "U1" H 920 4046 50  0000 R CNN
F 1 "DS18B20" H 920 3955 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 150 3750 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 1000 4250 50  0001 C CNN
	1    1150 4000
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Temperature:DS18B20 U2
U 1 1 5EAEFB35
P 2000 4000
F 0 "U2" H 1770 4046 50  0000 R CNN
F 1 "DS18B20" H 1770 3955 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 1000 3750 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 1850 4250 50  0001 C CNN
	1    2000 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5EAFF2DC
P 1150 4650
F 0 "#PWR0102" H 1150 4400 50  0001 C CNN
F 1 "GND" H 1155 4477 50  0000 C CNN
F 2 "" H 1150 4650 50  0001 C CNN
F 3 "" H 1150 4650 50  0001 C CNN
	1    1150 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5EB654E3
P 2000 3250
F 0 "R1" H 2070 3296 50  0000 L CNN
F 1 "2.7k" H 2070 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1930 3250 50  0001 C CNN
F 3 "~" H 2000 3250 50  0001 C CNN
	1    2000 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 4000 1600 4000
Wire Wire Line
	2300 4000 2400 4000
Text GLabel 3150 3000 2    50   Input ~ 0
DS18B20
Text GLabel 2700 3900 0    50   Input ~ 0
SDA
Text GLabel 2700 3800 0    50   Input ~ 0
SCL
Text GLabel 1700 2300 0    50   Output ~ 0
Relay1-Filter
Text GLabel 1700 1500 0    50   Output ~ 0
Relay2-ph
Text GLabel 1700 1600 0    50   Output ~ 0
Relay3-ch
Text GLabel 3300 2000 2    50   Output ~ 0
Relay4-light
Text GLabel 1700 1700 0    50   Input ~ 0
DS18B20
Text GLabel 1700 1800 0    50   Input ~ 0
SDA
Text GLabel 1700 1900 0    50   Input ~ 0
SCL
$Comp
L arduitouch:ESP32devkitC U3
U 1 1 5F1118C2
P 2550 800
F 0 "U3" H 2550 925 50  0000 C CNN
F 1 "ESP32devkitC" H 2550 834 50  0000 C CNN
F 2 "Package_DIP:DIP-40_W25.4mm_Socket_LongPads" H 2550 800 50  0001 C CNN
F 3 "" H 2550 800 50  0001 C CNN
	1    2550 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1800 2000 1800
Wire Wire Line
	1700 1900 2000 1900
Wire Wire Line
	3950 1500 3950 900 
Wire Wire Line
	1700 2300 2000 2300
$Comp
L power:+3.3V #PWR0101
U 1 1 5F1A2297
P 1150 3400
F 0 "#PWR0101" H 1150 3250 50  0001 C CNN
F 1 "+3.3V" H 1165 3573 50  0000 C CNN
F 2 "" H 1150 3400 50  0001 C CNN
F 3 "" H 1150 3400 50  0001 C CNN
	1    1150 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0103
U 1 1 5F1A2D0B
P 1550 750
F 0 "#PWR0103" H 1550 600 50  0001 C CNN
F 1 "+3.3V" H 1565 923 50  0000 C CNN
F 2 "" H 1550 750 50  0001 C CNN
F 3 "" H 1550 750 50  0001 C CNN
	1    1550 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 900  1550 750 
Wire Wire Line
	1550 900  2000 900 
$Comp
L power:GND #PWR0104
U 1 1 5F1AB679
P 3950 1650
F 0 "#PWR0104" H 3950 1400 50  0001 C CNN
F 1 "GND" H 3955 1477 50  0000 C CNN
F 2 "" H 3950 1650 50  0001 C CNN
F 3 "" H 3950 1650 50  0001 C CNN
	1    3950 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 1500 3950 1650
Connection ~ 3950 1500
Wire Wire Line
	3100 1500 3950 1500
Wire Wire Line
	3100 900  3950 900 
$Comp
L power:GND #PWR0105
U 1 1 5F1C3EDA
P 800 2300
F 0 "#PWR0105" H 800 2050 50  0001 C CNN
F 1 "GND" H 805 2127 50  0000 C CNN
F 2 "" H 800 2300 50  0001 C CNN
F 3 "" H 800 2300 50  0001 C CNN
	1    800  2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2200 800  2200
Wire Wire Line
	800  2200 800  2300
$Comp
L power:+5V #PWR0106
U 1 1 5F1CC92E
P 600 2600
F 0 "#PWR0106" H 600 2450 50  0001 C CNN
F 1 "+5V" H 615 2773 50  0000 C CNN
F 2 "" H 600 2600 50  0001 C CNN
F 3 "" H 600 2600 50  0001 C CNN
	1    600  2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2700 600  2700
Wire Wire Line
	600  2700 600  2600
Wire Wire Line
	2000 4500 3300 4500
Wire Wire Line
	1150 4300 1150 4500
Wire Wire Line
	2000 4300 2000 4500
Wire Wire Line
	2000 4500 1150 4500
Connection ~ 2000 4500
Wire Wire Line
	1150 4650 1150 4500
Connection ~ 1150 4500
Wire Wire Line
	2000 3500 1150 3500
Wire Wire Line
	2000 3500 3200 3500
Connection ~ 2000 3500
Connection ~ 1150 3500
Wire Wire Line
	1150 3500 1150 3700
Wire Wire Line
	2000 3500 2000 3700
Wire Wire Line
	1150 3400 1150 3500
Wire Wire Line
	1600 3000 2000 3000
Wire Wire Line
	1600 3000 1600 4000
Wire Wire Line
	2400 3000 2400 4000
Connection ~ 2400 3000
Wire Wire Line
	2400 3000 3150 3000
Wire Wire Line
	2000 3100 2000 3000
Connection ~ 2000 3000
Wire Wire Line
	2000 3000 2400 3000
Wire Wire Line
	2000 3400 2000 3500
$Comp
L Timer_RTC:DS3231M U4
U 1 1 5F1F6A20
P 3300 4000
F 0 "U4" H 3300 3511 50  0000 C CNN
F 1 "DS3231M" H 3300 3420 50  0000 C CNN
F 2 "Package_SO:SOIC-16W_7.5x10.3mm_P1.27mm" H 3300 3400 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS3231.pdf" H 3570 4050 50  0001 C CNN
	1    3300 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 4400 3300 4500
Wire Wire Line
	3200 3600 3200 3500
Wire Wire Line
	2700 3800 2800 3800
Wire Wire Line
	2700 3900 2800 3900
Text GLabel 3300 1000 2    50   Output ~ 0
MOSI
Wire Wire Line
	3100 1000 3300 1000
Text GLabel 3300 1100 2    50   Output ~ 0
RESET
Text GLabel 3300 1200 2    50   Output ~ 0
TxD
Text GLabel 3300 1400 2    50   Output ~ 0
BEEPER
Text GLabel 3300 1300 2    50   Input ~ 0
RxD
Text GLabel 3300 1600 2    50   Input ~ 0
MISO
Text GLabel 3300 1700 2    50   Output ~ 0
SCK
Text GLabel 3300 1800 2    50   Output ~ 0
CS
Text GLabel 3300 2100 2    50   Output ~ 0
DC
Text GLabel 3300 2400 2    50   Output ~ 0
LED_32
Text GLabel 3300 2300 2    50   Input ~ 0
T_IRQ
Wire Wire Line
	3300 1100 3100 1100
Wire Wire Line
	3300 1200 3100 1200
Wire Wire Line
	3300 1300 3100 1300
Wire Wire Line
	3300 1400 3100 1400
Wire Wire Line
	3300 1600 3100 1600
Wire Wire Line
	3300 1700 3100 1700
Wire Wire Line
	3300 1800 3100 1800
Wire Wire Line
	3300 2100 3100 2100
Wire Wire Line
	3300 2300 3100 2300
Wire Wire Line
	3300 2400 3100 2400
Text GLabel 1700 2000 0    50   Output ~ 0
T_CS
Wire Wire Line
	1700 2000 2000 2000
Text GLabel 1700 1100 0    50   Input ~ 0
A0
Wire Wire Line
	1700 1100 2000 1100
$Comp
L arduitouch:TFT T1
U 1 1 5F24AC21
P 8600 1550
F 0 "T1" H 10428 801 50  0000 L CNN
F 1 "TFT" H 10428 710 50  0000 L CNN
F 2 "" H 8600 1550 50  0001 C CNN
F 3 "" H 8600 1550 50  0001 C CNN
	1    8600 1550
	1    0    0    -1  
$EndComp
Text GLabel 8050 1700 0    50   Output ~ 0
T_IRQ
Text GLabel 8050 2200 0    50   Output ~ 0
MISO
Text GLabel 8050 2000 0    50   Input ~ 0
T_CS
Text GLabel 8050 2400 0    50   Input ~ 0
SCK
Text GLabel 8050 2500 0    50   Input ~ 0
MOSI
Text GLabel 8050 2600 0    50   Input ~ 0
DC
Text GLabel 8050 2700 0    50   Input ~ 0
RESET
Text GLabel 8050 2800 0    50   Input ~ 0
CS
Wire Wire Line
	8050 2200 8150 2200
Wire Wire Line
	8500 1800 8150 1800
Wire Wire Line
	8150 1800 8150 2200
Connection ~ 8150 2200
Wire Wire Line
	8150 2200 8500 2200
Wire Wire Line
	8050 2000 8500 2000
Wire Wire Line
	8050 1700 8500 1700
Wire Wire Line
	8050 2400 8250 2400
Wire Wire Line
	8500 2100 8250 2100
Wire Wire Line
	8250 2100 8250 2400
Connection ~ 8250 2400
Wire Wire Line
	8250 2400 8500 2400
Wire Wire Line
	8050 2500 8350 2500
Wire Wire Line
	8050 2600 8500 2600
Wire Wire Line
	8050 2700 8500 2700
Wire Wire Line
	8050 2800 8500 2800
Wire Wire Line
	8500 1900 8350 1900
Wire Wire Line
	8350 1900 8350 2500
Connection ~ 8350 2500
Wire Wire Line
	8350 2500 8500 2500
$Comp
L power:GND #PWR0107
U 1 1 5F25A700
P 8150 3100
F 0 "#PWR0107" H 8150 2850 50  0001 C CNN
F 1 "GND" H 8155 2927 50  0000 C CNN
F 2 "" H 8150 3100 50  0001 C CNN
F 3 "" H 8150 3100 50  0001 C CNN
	1    8150 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0108
U 1 1 5F25AA6B
P 8400 3100
F 0 "#PWR0108" H 8400 2950 50  0001 C CNN
F 1 "+3.3V" H 8415 3273 50  0000 C CNN
F 2 "" H 8400 3100 50  0001 C CNN
F 3 "" H 8400 3100 50  0001 C CNN
	1    8400 3100
	-1   0    0    1   
$EndComp
Wire Wire Line
	8150 3100 8150 2900
Wire Wire Line
	8150 2900 8500 2900
Wire Wire Line
	8500 3000 8400 3000
Wire Wire Line
	8400 3000 8400 3100
$Comp
L Transistor_BJT:BC557 Q1
U 1 1 5F261E61
P 6700 2400
F 0 "Q1" V 7028 2400 50  0000 C CNN
F 1 "BC557" V 6937 2400 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 6900 2325 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC557.pdf" H 6700 2400 50  0001 L CNN
	1    6700 2400
	0    1    -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5F26301F
P 6450 2700
F 0 "R2" V 6243 2700 50  0000 C CNN
F 1 "1K" V 6334 2700 50  0000 C CNN
F 2 "" V 6380 2700 50  0001 C CNN
F 3 "~" H 6450 2700 50  0001 C CNN
	1    6450 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	6600 2700 6700 2700
Wire Wire Line
	6700 2700 6700 2600
$Comp
L power:+3.3V #PWR0109
U 1 1 5F26DA46
P 6400 2200
F 0 "#PWR0109" H 6400 2050 50  0001 C CNN
F 1 "+3.3V" H 6415 2373 50  0000 C CNN
F 2 "" H 6400 2200 50  0001 C CNN
F 3 "" H 6400 2200 50  0001 C CNN
	1    6400 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2300 6400 2300
Wire Wire Line
	6400 2300 6400 2200
Text GLabel 6100 2700 0    50   Input ~ 0
LED_32
Wire Wire Line
	6100 2700 6300 2700
$Comp
L Connector:XLR3 J1
U 1 1 5F273787
P 1100 6150
F 0 "J1" H 1100 6515 50  0000 C CNN
F 1 "XLR3" H 1100 6424 50  0000 C CNN
F 2 "" H 1100 6150 50  0001 C CNN
F 3 " ~" H 1100 6150 50  0001 C CNN
	1    1100 6150
	1    0    0    -1  
$EndComp
$Comp
L Connector:XLR3 J2
U 1 1 5F274608
P 1950 6150
F 0 "J2" H 1950 6515 50  0000 C CNN
F 1 "XLR3" H 1950 6424 50  0000 C CNN
F 2 "" H 1950 6150 50  0001 C CNN
F 3 " ~" H 1950 6150 50  0001 C CNN
	1    1950 6150
	1    0    0    -1  
$EndComp
Text GLabel 3900 6250 2    50   Input ~ 0
DS18B20
Wire Wire Line
	750  6150 800  6150
Wire Wire Line
	1650 6150 1600 6150
$Comp
L power:GND #PWR03
U 1 1 5F27A9B4
P 1600 6250
F 0 "#PWR03" H 1600 6000 50  0001 C CNN
F 1 "GND" H 1605 6077 50  0000 C CNN
F 2 "" H 1600 6250 50  0001 C CNN
F 3 "" H 1600 6250 50  0001 C CNN
	1    1600 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 6600 1100 6450
Wire Wire Line
	1950 6450 1950 6600
$Comp
L power:+3.3V #PWR02
U 1 1 5F27FF19
P 1500 6050
F 0 "#PWR02" H 1500 5900 50  0001 C CNN
F 1 "+3.3V" H 1515 6223 50  0000 C CNN
F 2 "" H 1500 6050 50  0001 C CNN
F 3 "" H 1500 6050 50  0001 C CNN
	1    1500 6050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR04
U 1 1 5F280604
P 2350 6050
F 0 "#PWR04" H 2350 5900 50  0001 C CNN
F 1 "+3.3V" H 2365 6223 50  0000 C CNN
F 2 "" H 2350 6050 50  0001 C CNN
F 3 "" H 2350 6050 50  0001 C CNN
	1    2350 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 6050 2350 6150
Wire Wire Line
	2350 6150 2250 6150
Wire Wire Line
	1500 6050 1500 6150
Wire Wire Line
	1500 6150 1400 6150
$Comp
L power:GND #PWR01
U 1 1 5F286D8A
P 750 6250
F 0 "#PWR01" H 750 6000 50  0001 C CNN
F 1 "GND" H 755 6077 50  0000 C CNN
F 2 "" H 750 6250 50  0001 C CNN
F 3 "" H 750 6250 50  0001 C CNN
	1    750  6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  6250 750  6150
Wire Wire Line
	1600 6250 1600 6150
Wire Wire Line
	2400 6600 1950 6600
Connection ~ 1950 6600
Wire Wire Line
	1950 6600 1100 6600
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5F292659
P 2900 6250
F 0 "J3" H 2980 6292 50  0000 L CNN
F 1 "DS18B20_A" H 2980 6201 50  0000 L CNN
F 2 "" H 2900 6250 50  0001 C CNN
F 3 "~" H 2900 6250 50  0001 C CNN
	1    2900 6250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J4
U 1 1 5F2930A1
P 3150 7300
F 0 "J4" H 3230 7342 50  0000 L CNN
F 1 "Conn_01x03" H 3230 7251 50  0000 L CNN
F 2 "" H 3150 7300 50  0001 C CNN
F 3 "~" H 3150 7300 50  0001 C CNN
	1    3150 7300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR08
U 1 1 5F2A162A
P 2850 7100
F 0 "#PWR08" H 2850 6950 50  0001 C CNN
F 1 "+3.3V" H 2865 7273 50  0000 C CNN
F 2 "" H 2850 7100 50  0001 C CNN
F 3 "" H 2850 7100 50  0001 C CNN
	1    2850 7100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR07
U 1 1 5F2A2CFB
P 2650 7100
F 0 "#PWR07" H 2650 6950 50  0001 C CNN
F 1 "+5V" H 2665 7273 50  0000 C CNN
F 2 "" H 2650 7100 50  0001 C CNN
F 3 "" H 2650 7100 50  0001 C CNN
	1    2650 7100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5F2A363E
P 2850 7500
F 0 "#PWR09" H 2850 7250 50  0001 C CNN
F 1 "GND" H 2855 7327 50  0000 C CNN
F 2 "" H 2850 7500 50  0001 C CNN
F 3 "" H 2850 7500 50  0001 C CNN
	1    2850 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 7400 2850 7400
Wire Wire Line
	2850 7400 2850 7500
Wire Wire Line
	2950 7300 2650 7300
Wire Wire Line
	2650 7300 2650 7100
Wire Wire Line
	2950 7200 2850 7200
Wire Wire Line
	2850 7200 2850 7100
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 5F2AE5BC
P 3550 6250
F 0 "J5" H 3468 6567 50  0000 C CNN
F 1 "DS18B20_B" H 3468 6476 50  0000 C CNN
F 2 "" H 3550 6250 50  0001 C CNN
F 3 "~" H 3550 6250 50  0001 C CNN
	1    3550 6250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2400 6600 2400 6250
Wire Wire Line
	2400 6250 2700 6250
$Comp
L power:GND #PWR06
U 1 1 5F2B2F37
P 2600 6450
F 0 "#PWR06" H 2600 6200 50  0001 C CNN
F 1 "GND" H 2605 6277 50  0000 C CNN
F 2 "" H 2600 6450 50  0001 C CNN
F 3 "" H 2600 6450 50  0001 C CNN
	1    2600 6450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR05
U 1 1 5F2B34A4
P 2600 6050
F 0 "#PWR05" H 2600 5900 50  0001 C CNN
F 1 "+3.3V" H 2615 6223 50  0000 C CNN
F 2 "" H 2600 6050 50  0001 C CNN
F 3 "" H 2600 6050 50  0001 C CNN
	1    2600 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 6150 2600 6150
Wire Wire Line
	2600 6150 2600 6050
Wire Wire Line
	2700 6350 2600 6350
Wire Wire Line
	2600 6350 2600 6450
Wire Wire Line
	3900 6250 3750 6250
$Comp
L power:+3.3V #PWR011
U 1 1 5F2BE731
P 3900 6050
F 0 "#PWR011" H 3900 5900 50  0001 C CNN
F 1 "+3.3V" H 3915 6223 50  0000 C CNN
F 2 "" H 3900 6050 50  0001 C CNN
F 3 "" H 3900 6050 50  0001 C CNN
	1    3900 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5F2BFEB5
P 3850 6450
F 0 "#PWR010" H 3850 6200 50  0001 C CNN
F 1 "GND" H 3855 6277 50  0000 C CNN
F 2 "" H 3850 6450 50  0001 C CNN
F 3 "" H 3850 6450 50  0001 C CNN
	1    3850 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 6450 3850 6350
Wire Wire Line
	3850 6350 3750 6350
Wire Wire Line
	3750 6150 3900 6150
Wire Wire Line
	3900 6150 3900 6050
Wire Wire Line
	6900 2300 8500 2300
$Comp
L Connector_Generic:Conn_01x04 J8
U 1 1 5F2DFDAB
P 9950 5550
F 0 "J8" H 9868 5867 50  0000 C CNN
F 1 "Conn_01x04" H 9868 5776 50  0000 C CNN
F 2 "" H 9950 5550 50  0001 C CNN
F 3 "~" H 9950 5550 50  0001 C CNN
	1    9950 5550
	-1   0    0    -1  
$EndComp
$Sheet
S 10100 3550 650  650 
U 5F11BCC8
F0 "Sheet5F11BCC7" 50
F1 "extensionBoard.sch" 50
$EndSheet
Wire Wire Line
	1700 1700 2000 1700
Wire Wire Line
	1700 2100 2000 2100
Wire Wire Line
	3100 2000 3300 2000
Wire Wire Line
	1700 1600 2000 1600
Wire Wire Line
	1700 1500 2000 1500
$EndSCHEMATC
