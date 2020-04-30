# ardipool

## Goal

The goal of this project is to automatise some parts of my pool, mainly the flter pump based on the water temperature. There is also an option to use a ph pump.

## Hardware

The base hardware is an arduino mega rev3.
Other hardware:

- LCD: lcd 16x2
- Water temperatur: 1 or 2 DS18B20
- Ambient temperature: 1 DHT11
- Ethernet: 1 W5100 shield
- SDcard: included into the ethernet shield
- RTC: 1 DS1307
- Relay: 1 relay board (4 in my case)

## Funtionalities

- Read/write a config json config file on the SD card
- Use default values if no SD card or ethernet shield
- Auto-detect DS18B20
- Mesure temperature and evaluate filter time based on day hour.
- Get/Set values as json through http request
- Set time with ntp

## ToDo

- force filter pump
- fix temp for filter on 1 hour step
- set hour if no ethernet shield present
- add Hbridge driver for peristaltic pump (ph)
