#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#include "SwitchedStrip.h"
#include "SerialCommand.h"
#include "FS.h"

char jenTopic[] = "Upstairs/GJBedroom/Lights/BedSide/Jen";
char gregTopic[] = "Upstairs/GJBedroom/Lights/BedSide/Greg";

SwitchedStrip s1 = SwitchedStrip(D0,30,D1,110,50,10,jenTopic); //J
SwitchedStrip s2 = SwitchedStrip(D2,29,D3,80,80,30,gregTopic); //Me

char mqtt_server[] = "192.168.4.158";
char name[32] = "BedLights-";
String mac = WiFi.macAddress();

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

struct Settings {
	char SSID[32];
	char PSK[32];
	char OtaPassword[32];
} settings = { "SSID","PSK","OTAPW" };

void saveConfig(){
	bool result = SPIFFS.begin();
	Serial.println("SPIFFS opened:" + result);
	File f = SPIFFS.open("config.txt", "w");
	if (!f) {
		Serial.println("File creation failed");
	}
	f.printf("ssid=%s\n", settings.SSID);
	f.printf("psk=%s\n", settings.PSK);
	f.printf("otaPw=%s\n", settings.OtaPassword);
	f.close();
	SPIFFS.end();
}
void loadConfig() {
	SPIFFS.begin();
	File f = SPIFFS.open("config.txt", "r");
	if (f) {
		while (f.available()) {
			String key = f.readStringUntil('=');
			String value = f.readStringUntil('\n');
			char* target = 0;
			if (key.equals("ssid")){
				target = settings.SSID;
				Serial.print("Loaded SSID: ");
				Serial.println(value);
			}
			else if (key.equals("psk")) {
				target = settings.PSK;
			}
			else if (key.equals("otaPw")) {
				target = settings.OtaPassword;
			}
			else {
				Serial.print("Unknown key: ");
				Serial.println(key);
			}
			if (target) {
				value.toCharArray(target, 32);
			}
		}
	}
	else {
		Serial.println("Config not found");
	}
	SPIFFS.end();
}
void setup()
{
	mac.toCharArray(name+10, 20);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	Serial.begin(115200);
	Serial.printf("Booting %s...\n", name);
	loadConfig();
	setupWifi();
	setupOTA();
}
void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch(event) {
        case WIFI_EVENT_STAMODE_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case WIFI_EVENT_STAMODE_DISCONNECTED:
            Serial.println("WiFi lost connection");
            break;
    }
}
void setupWifi() {
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(settings.SSID);
	WiFi.onEvent(WiFiEvent);
	WiFi.begin(settings.SSID, settings.PSK);
	setup_mqtt();
}
void setupOTA() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(name);

  // No authentication by default
  ArduinoOTA.setPassword(settings.OtaPassword);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}
void setup_mqtt() {
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	s1.OnSwitch(switchChanged);
	s2.OnSwitch(switchChanged);
}
void switchChanged(bool state, char*name) {
	client.publish(name, state ? "ON" : "OFF");
}
void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("MQTT ");
	Serial.print(topic);
	Serial.print(": ");
	for (int i = 0; i < length; i++) {
		Serial.print((char) payload[i]);
	}
	Serial.print(length);
	Serial.println(".");
	payload[length] = 0;
	if (strcmp(topic, "test")==0) {
		if (strcmp((char*)payload, "ON") == 0)
		{
			digitalWrite(LED_BUILTIN, LOW);
		}
		if (strcmp((char*)payload, "OFF") == 0)
		{
			digitalWrite(LED_BUILTIN, HIGH);
		}
	}
	else if (strcmp(topic, jenTopic)) {
		if (strcmp((char*)payload, "ON") == 0)
		{
			s1.On(true);
		}
		if (strcmp((char*)payload, "OFF") == 0)
		{
			s1.On(false);
		}
		if (strcmp((char*)payload, "TOGGLE") == 0)
		{
			s1.On(!s1.GetState());
		}
	}
	else if (strcmp(topic, gregTopic)) {
		if (strcmp((char*)payload, "ON") == 0)
		{
			s2.On(true);
		}
		if (strcmp((char*)payload, "OFF") == 0)
		{
			s2.On(false);
		}
		if (strcmp((char*)payload, "TOGGLE") == 0)
		{
			s2.On(!s2.GetState());
		}
	}
}

long lastMqttAttempt = 0;
void check_mqtt() {
	if (!client.connected()) {
		long now = millis();
		if (now - lastMqttAttempt > 5000) {
			lastMqttAttempt = now;
			if (reconnect()) {
				lastMqttAttempt = 0;
			}
		}
	}
	else {
		//MQTT Fine.
	}
}
boolean reconnect() {
	if (WiFi.isConnected() && client.connect(name)) {
		client.publish("test", "Bed Lights are here!");
		client.subscribe("test");
		client.subscribe(jenTopic);
		client.subscribe(gregTopic);
	}
	boolean res = client.connected();
	Serial.print("Attempted MQTT connection: ");
	Serial.println(res ? "Success" : "Fail");
	return res;
}

void loop()
{
	ArduinoOTA.handle();
	check_mqtt();
	client.loop();
	s1.ProcessInput();
	s2.ProcessInput();
	s1.Animate();
	s2.Animate();
    if(!s1.Animating() && !s2.Animating())
	{
      delay(50);
    }
}
