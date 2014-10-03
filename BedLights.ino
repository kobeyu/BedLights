#include <SoftwareSerial.h>

#include "SwitchedStrip.h"
#include "SerialCommand.h"

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
        SCmd.addCommand("TOGGLE", switchToggle);
        SCmd.addCommand("SETCOLOUR", setColour);
        SCmd.addDefaultHandler(badFunc);

}
void badFunc(){
  Serial.println("BAD");
  ss.println("BAD");
}
void switchToggle()
{
 	char *arg;
	arg = SCmd.next();
	if (arg != NULL){
		if (strcmp(arg,"1")==0){
			s1.On(!s1.GetState());
                      	ss.println("OK: Toggle 1");
		}
		else if (strcmp(arg,"2")==0){
			s2.On(!s2.GetState());
                        ss.println("OK: Toggle 2");
		}
                else {
                  ss.print("FAIL: Toggle: unknown arg: ");
                  ss.println(arg);
                }
	}else
        {
          ss.println("FAIL: Toggle: no arg found");
        }
} 
void setColour()
{
    Serial.println("GOt here 1");
    int strip = atoi(SCmd.next());
    Serial.println("GOt here 1");
    int r = atoi(SCmd.next());
    Serial.println("GOt here 1");
    int g = atoi(SCmd.next());
    Serial.println("GOt here 1");
    int b = atoi(SCmd.next());
    Serial.println("GOt here 1");
    
    if(strip == 1){
     s1.SetTargetColour(r,g,b); 
    ss.println("OK");
    }else if(strip == 2){
    Serial.println("GOt here 1");
     s2.SetTargetColour(r,g,b); 
    Serial.println("GOt here 1");
     ss.print("OK: SetColour 2 R:");
    Serial.println("GOt here 1");
     ss.print(r);
    Serial.println("GOt here 1");
     ss.print(" g:");
    Serial.println("GOt here 1");
     ss.print(g);
    Serial.println("GOt here 1");
     ss.print(" b:");
    Serial.println("GOt here 1");
     ss.println(b);
    Serial.println("GOt here 1");
    }else {
      ss.println("FAIL: SetColour: Unknown strip");
    }
}
void switchOn()
{
	int arg = atoi(SCmd.next());
	if (arg != NULL){
		if (arg == 1){
			s1.On(true);
		}
		else if (arg == 2){
			s2.On(true);
		}
	}
	ss.println("OK");
}
void switchOff()
{
	int arg = atoi(SCmd.next());
	if (arg != NULL){
		if (arg == 1){
			s1.On(false);
		}
		else if (arg == 2){
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
        if(!s1.Animating() && !s2.Animating()){
          delay(50);
        }
}
