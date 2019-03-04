BedLights
=========

Arduino code running in my bedside lights.

There is:

* an arduino
* 2 switches (internal pullup resistors, switched to ground) (pins 9 and 10)
* 2 Neopixel strips (one with 29 LEDs, one with 30) (pins 5 and 6)
* A bluetooth serial chip. (BT TX / Arduino RX on pin 2, BT RX / Arduino TX on pin 3)

Currently the code animates the lights on and off when the switches are switched. The two strips each have a single colour, hardcoded. The animation runs the brightness from 0 to 1. The brightness is also affected by how far from the center the LED is - so it's brightest in the middle, and barely on at the edge.

Obsoleted 2019
==============

In February 2019 I replaced this code with the excellent [ESP Home](https://esphome.io) and a 20 line config script. The animations aren't custom any more, but it has automatic wifi strength, online status and discovery in my [hassio](https://home-assistant.io) system.
