#include "SwitchedStrip.h"
#include "SerialCommand.h"
#include <SoftwareSerial\SoftwareSerial.h>

SwitchedStrip s1 = SwitchedStrip(5,30,9,110,50,10,HIGH); //J
SwitchedStrip s2 = SwitchedStrip(6,29,10,80,80,30,LOW); //Me

SoftwareSerial ss = SoftwareSerial(2, 3);
SerialCommand SCmd(ss);

void setup()
{
	Serial.begin(57600);
	ss.begin(9600);

	SCmd.addCommand("ON", switchOn);
	SCmd.addCommand("OFF", switchOff);

}

void switchOn()
{
	char *arg;
	arg = SCmd.next();
	if (arg != NULL){
		if (arg == "1"){
			s1.On(true);
		}
		else if (arg == "2"){
			s2.On(true);
		}
	}
	ss.println("OK");
}
void switchOff()
{
	char *arg;
	arg = SCmd.next();
	if (arg != NULL){
		if (arg == "1"){
			s1.On(false);
		}
		else if (arg == "2"){
			s2.On(false);
		}
	}
	ss.println("OK");
}


void loop()
{
	SCmd.readSerial();
	s1.ProcessInput();
	s2.ProcessInput();
	s1.Animate();
	s2.Animate();
}
