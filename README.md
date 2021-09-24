# WiFiScanner
A scanner to save and analyse WiFi networks

## Acknowledgement
This project is based on https://au.mathworks.com/help/thingspeak/measure-arduino-wifi-signal-strength-with-esp32.html

## What's in it?
I added an easy to use scanner box, which takes into account the limits imposed by the Thingspeak free tier service (accepting a new record in 15 seconds intervals). 

## How it works
### Setup mode
The box firstly attempts to connect to a known WiFi network. If it can't connect to a known network, it will activate a WiFi Access-Point called "WiFi Scanner", which will allow a user to provide password for a known network.
### Setting Location
The user define the incremental number of the locations of measurements (1,2,3...), and select that number on the box, prior to making a scan.
### Scanning 
When scanning, the box will move in and out of scanning-WiFi and connected-to-WiFi states, as it "listens" to the WiFi networks measuring their reception level. Then it will connect to the one network it already knows, in order to send the data to the cloud, to a predefined channel in Thingspeak.com (although its easy to change, and submit the data elsewhere).
### Analysing
Data is accomulated in Thingspeak.com channel, and analysed using matlab script to produce a heat map of the house WiFi network. 
The more measurements are made the better accuracy, because each point is calculated according to it's average measurement.
### Map image
It is required to decide upfront where to measure the network around the house, and to provide these coordinates on the house image. Any image will do, but try to make it to scale.

---
### More Info
A DEMO video can be seen here: https://youtu.be/mV1DMqeotEE


The project uses ESP32 like this one: https://www.mischianti.org/wp-content/uploads/2021/07/ESP32-DEV-KIT-DevKitC-v4-pinout-mischianti.jpg

