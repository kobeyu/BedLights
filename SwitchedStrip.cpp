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
		float animProgress = 1 - ((float)animationRemaining / ANIMATION_DURATION);
		float animBrightness = _lastSwitchState ? animProgress : 1 - animProgress;
                int pixels = _strip->numPixels();
                Serial.print("Brights (");
                Serial.print(animBrightness);
                Serial.print("): ");
		for(int light = 0; light < pixels;light++){
			float posBrightness = 2 * ((float)light+1) / (pixels+1);
                        if(posBrightness > 1){
                           posBrightness = 2 - posBrightness; 
                        }
                        float brightness = posBrightness * animBrightness;
                        Serial.print(brightness);
                        Serial.print(" ");
			_strip->setPixelColor(light,brightness*_r,brightness*_g,brightness*_b);
		}
                Serial.println(".");
		_strip->show();
	}
}
void SwitchedStrip::ProcessInput(void){
	bool readVal = digitalRead(_switchPin) == _onState; 
	if(readVal != _lastSwitchState){
                //If we're currently in an animation, then shorten the next animation appropriately.
                //(We want 0 1 2 1 0 not 0 1 2 10 9 8)
                int animDur = ANIMATION_DURATION;
                unsigned long t = millis();
                if(t <= animationFinishAt){
                   animDur -= (animationFinishAt - t); 
                }
		animationFinishAt = t + animDur;
		_lastSwitchState = readVal;
	}
}
