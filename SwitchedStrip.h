#include <Adafruit_NeoPixel.h>
class SwitchedStrip
{
public:
	SwitchedStrip(int ledPin,int ledCount, int switchPin,int r,int g, int b,char* name);
	void ProcessInput(void);
	void Animate(void);
	void On(bool onOrOff);
	bool Animating(void);
	bool GetState(void);
	void SetTargetColour(int r,int g, int b);
	void OnSwitch(void(*switchCallback)(bool, char*));
private:
	void OnInternal(bool onOrOff,bool doCallback);
	void WriteToLeds(float animProgress);  
	void WriteToLedsWipe(float animProgress);  
	void WriteToLedsFade(float animProgress);
	float posBrightness(int light,int pixels);    
	Adafruit_NeoPixel * _strip;
	char* _name;
	void (*_switchCallback)(bool, char*);
	int _switchPin;
	int _lastSwitchState;
	bool _targetLightState;
	int _r;
	int _g;
	int _b;
	unsigned long lastAnimate;
	unsigned long animationFinishAt;
};

