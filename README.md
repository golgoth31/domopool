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

### Configuration

The json configuration file is automaticaly genarated on the sdcard with safe default values. If the file as been put on the card before the first boot, it will be used.
To ease configuration update 2 front-end are under developement:

- CLI: [https://github.com/golgoth31/domopool-cli](https://github.com/golgoth31/domopool-cli)
- UI: [https://github.com/golgoth31/domopool-cli/web](https://github.com/golgoth31/domopool-cli/web)

### Temp/filter time

The local table is defined like this:


| Temp  |Hour|0  |1  |2  |3  |4  |5  |6  |7  |8  |9  |10 |11 |12 |13 |14 |15 |16 |17 |18 |19 |20 |21 |22 |23 |
|-------|----|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|  t<1  |    | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X |
|1<t<3  |    |   |   |   | X | X |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|3<t<6  |    |   |   |   | X | X |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|6<t<9  |    |   |   |   | X | X | X |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|9<t<12 |    |   |   |   |   |   |   |   |   | X | X | X | X |   |   |   |   |   |   |   |   |   |   |   |   |
|12<t<15|    |   |   |   |   |   |   |   |   | X | X | X |   |   |   | X | X |   |   |   |   |   |   |   |   |
|15<t<18|    |   |   |   |   |   |   |   |   | X | X | X |   |   |   | X | X | X |   |   |   |   |   |   |   |
|18<t<21|    |   |   |   |   |   |   |   |   | X | X | X | X |   |   | X | X | X |   |   |   |   |   |   |   |
|21<t<23|    |   |   |   |   |   |   |   |   | X | X | X | X |   |   | X | X | X | X |   |   |   |   |   |   |
|23<t<26|    |   |   |   | X | X |   |   |   | X | X | X | X |   |   | X | X | X | X | X |   |   |   |   |   |
|26<t<28|    |   |   |   | X | X | X |   |   | X | X | X | X |   |   | X | X | X | X | X | X |   |   |   |   |
|28<t<30|    |   |   |   | X | X | X | X |   | X | X | X | X |   |   | X | X | X | X | X | X | X |   |   |   |
|30<t   |    | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X |

Other table exemples:

- from [1](https://pool-technologie.com/fichiers/spec_telechargement/MANUEL%20SIMPLEO-FR.pdf)

- from [https://www.piscine-center.net/gestion-de-la-filtration-et-de-l-electrolyseur.html](https://www.piscine-center.net/gestion-de-la-filtration-et-de-l-electrolyseur.html)

    ![](https://uppict.piscine-center.net/fiche/12832/automateau-gestion-de-la-filtration-et-de-l-electrolyseur-piscine-center-95319400.jpg)

- from [https://www.perfeco-f.fr/index.php/le-perfeco](https://www.perfeco-f.fr/index.php/le-perfeco)

    ![](https://clweb01.hosteur.com/~perfecotest.fr/images/filtration.gif)



## ToDo

- force filter pump for limited time
- set hour if no ethernet shield present
- add Hbridge driver for peristaltic pump (ph)
- add ability to redefine filter pump steps or
