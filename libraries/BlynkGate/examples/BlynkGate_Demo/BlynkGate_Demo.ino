#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_AUTH_TOKEN            "xxxxxxxxxxxxxxxxxxxxxxx_xxxxxxxx"

#include "BlynkGate.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wifi name (2.4G)";     // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "Wifi pass"; // Key in your wifi password.

#define PIN_LED   13
#define PIN_SEN   A2

unsigned long lastTimeSen = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Start BlynkGate_Demo"));

  Blynk.begin(auth, ssid, pass);

  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
	Blynk.run();
  // DO NOT using delay 
  // delay(100);

  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if(millis() - lastTimeSen >= 10000){
    lastTimeSen = millis();
    float tempValue = analogRead(PIN_SEN);
      Blynk.virtualWrite(1, tempValue);
      Blynk.virtualWrite(0, tempValue);
  }
  
}


BLYNK_WRITE_DEFAULT(){
    int myInt = param.asInt();

    Serial.print("input V");
    Serial.print(request.pin);
    Serial.print(": ");
    Serial.println(myInt);

    if(request.pin == 2) digitalWrite(PIN_LED, myInt);

}
