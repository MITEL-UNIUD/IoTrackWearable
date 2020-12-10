# IoTrackWearable
We have taken an IoT device and we haved adapted it to be part of a SmartHealth Ambient Assisted Living system.

The IoT device implements fall detection and geofencing.

*Fall detection thresholds*:

- Minati [[1]](#1) (default);
- Lim et al. variant [[2]](#2)

*Geofencing*:

- circumference geo fence (default);
- polygonal geo fence.

## Resources
- a folder containing the files used to generate the firmware (**LGT-92_-LoRa_GPS_Tracker-master**);
- a file used to test a polygonal geo fence (**complexGeofence.c**);
- a file containing the Decoder function for The Things Network (**DecoderFunctionTTN.txt**).

## How to do?
After installed [Keil $\mu$Vision5](http://www2.keil.com/mdk5/uvision/), double click on **Lora.uvprojx** located in *LGT-92_-LoRa_GPS_Tracker-master\Projects\Multi\Applications\LoRa\DRAGINO-LRWAN(AT)\MDK-ARM\STM32L072CZ-Nucleo*. 

Now you can analyse and edit the source code!

As you will see in the source code, you can choose your prefer threshold and geo fence. 

Remember to change the coordinates of the geo fence with **YOUR** coordinates (we use a specific building as default)!

After rebuilding all the files, the generated file **NUCLEO_CUBE_LORA.hex** is located in *LGT-92_-LoRa_GPS_Tracker-master\Projects\Multi\Applications\LoRa\DRAGINO-LRWAN(AT)\MDK-ARM\STM32L072CZ-Nucleo\sx1276mb1las*.

## Payload
The Decoder function is based on the payload of the IoT device, which it has a payload compatible with the Globalsat LT-100 device.
This is the payload:

| Global Sat Device Type|      GPS-fix Status \& Report Type|   Battery Capacity|Latitude|Longitude|
|-------------|:-------------:|-------:|------:| ------:|
| 1 byte |  1 byte | 1 byte |  4 bytes | 4 bytes |


## References
<a id="1">[1]</a> 

Minati Veronica. 

Analisi dei dati raccolti su una simulazione di cadute. 

Relazione tecnica,

Università degli Studi di Udine, Via delle Scienze, 206, 33100 Udine UD.

<a id="2">[2]</a> 

Dongha Lim, Chulho Park, Nam Kim, Sang-Hoon Kim, e Yun Seop Yu. 

Fall-detection algorithm using 3-axis acceleration: Combination with simple threshold and hidden markov model. 

Journal of Applied Mathematics, 2014, 09 2014.s