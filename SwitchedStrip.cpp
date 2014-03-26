#include "SwitchedStrip.h"
#include "Adafruit_NeoPixel.h"
#define MILLIS_BETWEEN_FRAMES 16 //approx 60fps.
#define ANIMATION_DURATION 5000 //animation takes 5s.

SwitchedStrip::SwitchedStrip(int ledPin,int ledCount, int switchPin,int r, int g, int b,int onState){
	_lastSwitchState = false;
	_strip = new Adafruit_NeoPixel(ledCount,ledPin, NEO_GRB + NEO_KHZ800);
        _strip->begin();
        _strip->setBrightness(100);
	_switchPin = switchPin;
        _onState = onState;
	pinMode(_switchPin,INPUT_PULLUP);
	_r = r;
	_g = g;
	_b = b;
}
void SwitchedStrip::Animate(void){
	unsigned long t = millis();
	if(t - lastAnimate > MILLIS_BETWEEN_FRAMES && t <= animationFinishAt){
		lastAnimate = t;
		unsigned long animationRemaining = animationFinishAt - t;
		float animProgress = 1 -  ((float)animationFinishAt - t / ANIMATION_DURATION);
		float currBrightness = _lastSwitchState ? animProgress : 1 - animProgress;
		int centerLed = _strip->numPixels() / 2;
		for(int light = 0; light < _strip->numPixels();light++){
			float scale;
			if(light<=centerLed){
				scale = (float) (light+1) / (centerLed+1);
			}else{
				scale = 1 + (float) (centerLed - light) / (centerLed+1);
			}
			scale *= currBrightness;
			_strip->setPixelColor(light,scale*_r,scale*_g,scale*_b);
		}
		_strip->show();
	}
}
void SwitchedStrip::ProcessInput(void){
	bool readVal = digitalRead(_switchPin) == _onState; 
	if(readVal != _lastSwitchState){
		animationFinishAt = millis() + ANIMATION_DURATION;
		_lastSwitchState = readVal;
	}
}
