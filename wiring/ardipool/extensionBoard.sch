EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x04 J6
U 1 1 5F145678
P 7450 3150
F 0 "J6" H 7530 3142 50  0000 L CNN
F 1 "Relay" H 7530 3051 50  0000 L CNN
F 2 "" H 7450 3150 50  0001 C CNN
F 3 "~" H 7450 3150 50  0001 C CNN
	1    7450 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 5F14567E
P 7600 4400
AR Path="/5F14567E" Ref="J?"  Part="1" 
AR Path="/5F11BCC8/5F14567E" Ref="J7"  Part="1" 
F 0 "J7" H 7680 4392 50  0000 L CNN
F 1 "Analog" H 7680 4301 50  0000 L CNN
F 2 "" H 7600 4400 50  0001 C CNN
F 3 "~" H 7600 4400 50  0001 C CNN
	1    7600 4400
	1    0    0    -1  
$EndComp
Text GLabel 4350 3400 0    50   Input ~ 0
Relay1-Filter
Text GLabel 4350 3500 0    50   Input ~ 0
Relay2-ph
Text GLabel 4350 3600 0    50   Input ~ 0
Relay3-ch
Text GLabel 4350 3700 0    50   Input ~ 0
Relay4-light
$Comp
L Logic_LevelTranslator:TXS0108EPW U5
U 1 1 5F145688
P 5400 3700
F 0 "U5" H 5400 2911 50  0000 C CNN
F 1 "TXS0108EPW" H 5400 2820 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 5400 2950 50  0001 C CNN
F 3 "www.ti.com/lit/ds/symlink/txs0108e.pdf" H 5400 3600 50  0001 C CNN
	1    5400 3700
	1    0    0    -1  
$EndComp
$Comp
L Analog_ADC:ADS1115IDGS U6
U 1 1 5F14568E
P 6800 4400
F 0 "U6" H 6800 5081 50  0000 C CNN
F 1 "ADS1115IDGS" H 6800 4990 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 6800 3900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 6750 3500 50  0001 C CNN
	1    6800 4400
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0110
U 1 1 5F145694
P 5300 2900
F 0 "#PWR0110" H 5300 2750 50  0001 C CNN
F 1 "+3.3V" H 5315 3073 50  0000 C CNN
F 2 "" H 5300 2900 50  0001 C CNN
F 3 "" H 5300 2900 50  0001 C CNN
	1    5300 2900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0111
U 1 1 5F14569A
P 5500 2900
F 0 "#PWR0111" H 5500 2750 50  0001 C CNN
F 1 "+5V" H 5515 3073 50  0000 C CNN
F 2 "" H 5500 2900 50  0001 C CNN
F 3 "" H 5500 2900 50  0001 C CNN
	1    5500 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2900 5500 3000
Wire Wire Line
	5300 2900 5300 3000
$Comp
L power:GND #PWR0112
U 1 1 5F1456A2
P 5400 4750
F 0 "#PWR0112" H 5400 4500 50  0001 C CNN
F 1 "GND" H 5405 4577 50  0000 C CNN
F 2 "" H 5400 4750 50  0001 C CNN
F 3 "" H 5400 4750 50  0001 C CNN
	1    5400 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4750 5400 4400
Wire Wire Line
	5800 3400 6100 3400
Wire Wire Line
	6100 3400 6100 3050
Wire Wire Line
	6100 3050 7250 3050
Wire Wire Line
	5800 3500 6200 3500
Wire Wire Line
	6200 3500 6200 3150
Wire Wire Line
	6200 3150 7250 3150
Wire Wire Line
	5800 3600 6300 3600
Wire Wire Line
	6300 3600 6300 3250
Wire Wire Line
	6300 3250 7250 3250
Wire Wire Line
	5800 3700 6400 3700
Wire Wire Line
	6400 3700 6400 3350
Wire Wire Line
	6400 3350 7250 3350
Wire Wire Line
	5800 3800 6200 3800
Wire Wire Line
	6200 3800 6200 4400
Wire Wire Line
	5800 3900 6100 3900
Wire Wire Line
	5800 4000 6000 4000
Wire Wire Line
	5800 4100 5900 4100
Wire Wire Line
	5900 4100 5900 4200
Wire Wire Line
	7200 4300 7400 4300
Wire Wire Line
	7200 4400 7400 4400
Wire Wire Line
	7200 4500 7400 4500
Wire Wire Line
	7200 4600 7400 4600
Wire Wire Line
	4350 3400 5000 3400
Wire Wire Line
	4350 3500 5000 3500
Wire Wire Line
	4350 3600 5000 3600
Wire Wire Line
	4350 3700 5000 3700
Text GLabel 4350 3800 0    50   Input ~ 0
SCL
Text GLabel 4350 3900 0    50   Input ~ 0
SDA
Wire Wire Line
	4350 3800 5000 3800
Wire Wire Line
	4350 3900 5000 3900
Wire Wire Line
	6400 4400 6200 4400
Wire Wire Line
	6100 4500 6400 4500
Wire Wire Line
	6100 3900 6100 4500
Wire Wire Line
	6000 4600 6400 4600
Wire Wire Line
	6000 4000 6000 4600
Wire Wire Line
	6400 4200 5900 4200
$EndSCHEMATC
