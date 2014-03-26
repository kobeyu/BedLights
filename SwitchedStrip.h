#include "Adafruit_NeoPixel.h"
class SwitchedStrip
{
public:
	SwitchedStrip(int ledPin,int ledCount, int switchPin,int r,int g, int b,int onState);
	void ProcessInput(void);
	void Animate(void);
private:
	Adafruit_NeoPixel * _strip;
	int _switchPin;
        int _onState;
	bool _lastSwitchState;
	int _r;
	int _g;
	int _b;
	unsigned long lastAnimate;
	unsigned long animationFinishAt;
};

