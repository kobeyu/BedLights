#include "Adafruit_NeoPixel.h"
class SwitchedStrip
{
public:
	SwitchedStrip(int ledPin,int ledCount, int switchPin,int r,int g, int b,int onState);
	void ProcessInput(void);
	void Animate(void);
	void On(bool onOrOff);
        bool Animating(void);
        bool GetState(void);
        void SetTargetColour(int r,int g, int b);
private:
        void WriteToLeds(float animProgress);  
        void WriteToLedsWipe(float animProgress);  
        void WriteToLedsFade(float animProgress);
        float posBrightness(int light,int pixels);    
	Adafruit_NeoPixel * _strip;
	int _switchPin;
        int _onState;
	bool _lastSwitchState;
	bool _targetLightState;
	int _r;
	int _g;
	int _b;
	unsigned long lastAnimate;
	unsigned long animationFinishAt;
};

