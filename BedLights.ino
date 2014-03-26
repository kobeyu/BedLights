
#include "SwitchedStrip.h"

SwitchedStrip s1 = SwitchedStrip(5,30,9,110,50,10,HIGH);
SwitchedStrip s2 = SwitchedStrip(6,29,10,80,80,30,LOW);

void setup()
{
	Serial.begin(57600);
}


void loop()
{
	s1.ProcessInput();
	s2.ProcessInput();
	s1.Animate();
	s2.Animate();
}
