#include "SwitchedStrip.h"
#include <Adafruit_NeoPixel.h>
#define MILLIS_BETWEEN_FRAMES 16 //approx 60fps.
#define ANIMATION_DURATION 5000 //animation takes 5s.

SwitchedStrip::SwitchedStrip(int ledPin, int ledCount, int switchPin, int r, int g, int b,char* name){
	_name = name;
	_targetLightState = false;
	_strip = new Adafruit_NeoPixel(ledCount, ledPin, NEO_GRB + NEO_KHZ800);
	_strip->begin();
	_strip->setBrightness(200);
	_switchPin = switchPin;
	pinMode(_switchPin, INPUT_PULLUP);
	SetTargetColour(r,g,b);
	_lastSwitchState = false;// digitalRead(_switchPin) == HIGH;
}
void SwitchedStrip::SetTargetColour(int r, int g, int b){
	_r = r;
	_g = g;
	_b = b;
	OnInternal(GetState(),false);
	Serial.print("Set Colour: r:");
	Serial.print(r);
	Serial.print(" g:");
	Serial.print(g);
	Serial.print(" b:");
	Serial.println(b);
}
bool SwitchedStrip::GetState(){
  return _targetLightState;
}
void SwitchedStrip::OnSwitch(void(*switchCallback)(bool, char*)) {
	_switchCallback = switchCallback;
}
void SwitchedStrip::On(bool onOrOff) {
	OnInternal(onOrOff, false);
}
void SwitchedStrip::OnInternal(bool onOrOff,bool doCallback){
	//If we're currently in an animation, then shorten the next animation appropriately.
	//(We want 0 1 2 1 0 not 0 1 2 10 9 8)
	if (onOrOff == _targetLightState) return;
	int animDur = ANIMATION_DURATION;
	unsigned long t = millis();
	if (t <= animationFinishAt){
		animDur -= (animationFinishAt - t);
	}
	animationFinishAt = t + animDur;
	_targetLightState = onOrOff;
	if (doCallback && _switchCallback) {
		_switchCallback(onOrOff, _name);
	}
}
bool SwitchedStrip::Animating(void){
   unsigned long t = millis();
   return t <= animationFinishAt;
}
void SwitchedStrip::Animate(void){
	unsigned long t = millis();
	if (t - lastAnimate > MILLIS_BETWEEN_FRAMES && t <= animationFinishAt){
		lastAnimate = t;
		unsigned long animationRemaining = animationFinishAt - t;
		float animProgress = 1 - ((float)animationRemaining / ANIMATION_DURATION);
                WriteToLeds(_targetLightState ? animProgress : 1 - animProgress);
	}
}
float SwitchedStrip::posBrightness(int light,int pixels){
      float posBrightness = 2 * ((float)light+1) / (pixels + 1);
      if (posBrightness > 1){
         posBrightness = 2 - posBrightness; 
      }
      return posBrightness;
}
void SwitchedStrip::WriteToLeds(float animProgress){
  WriteToLedsWipe(animProgress);
  _strip->show();  
}
void SwitchedStrip::WriteToLedsWipe(float animProgress){
    int pixels = _strip->numPixels();
    int lastLit = pixels * (animProgress+0.01);
    for (int li = 0; li < pixels; li++){
      float a = constrain(animProgress*pixels - li,0,1);
      float b = a * posBrightness(li,pixels);
      _strip->setPixelColor(li,b*_r,b*_g,b*_b);
    }
}
void SwitchedStrip::WriteToLedsFade(float animProgress){
	int pixels = _strip->numPixels();
	for (int light = 0; light < pixels; light++){
		float brightness = posBrightness(light,pixels) * animProgress;
		_strip->setPixelColor(light, brightness*_r, brightness*_g, brightness*_b);
	}
	_strip->show();  
}
void SwitchedStrip::ProcessInput(void){
	int read = digitalRead(_switchPin);
	if (read != _lastSwitchState){
		_lastSwitchState = read;
		OnInternal(!_targetLightState,true);
		Serial.println("Switched.");
	}
}
