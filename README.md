BedLights
=========

Arduino code running in my bedside lights.

There is:

* an arduino
* 2 switches (internal pullup resistors, switched to ground)
* 2 Neopixel strips (one with 29 LEDs, one with 30)
* A bluetooth serial chip.

Currently the code animates the lights on and off when the switches are switched. The two strips each have a single colour, hardcoded. The animation runs the brightness from 0 to 1. The brightness is also affected by how far from the center the LED is - so it's brightest in the middle, and barely on at the edge.

