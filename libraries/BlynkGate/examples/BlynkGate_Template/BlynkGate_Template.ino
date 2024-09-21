// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_AUTH_TOKEN            "xxxxxxxxxxxxxxxxxxxxxxx_xxxxxxxx"

// Step 2: include library
#include "BlynkGate.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wifi name (2.4G)";     // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "Wifi pass"; // Key in your wifi password.

unsigned long lastTimeSen = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_BTN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));
  
  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
	Blynk.run();
  // DO NOT using delay 
  // delay(100);

  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if(millis() - lastTimeSen >= 10000){
    lastTimeSen = millis();
    // Step 6: Send Virtual pin Value
    Blynk.virtualWrite(0, analogRead(A0));
  }
  
}

// Step 5: Get Virtual pin Value
BLYNK_WRITE_DEFAULT(){
    // double myDouble = param.asFloat();
    // String myString = param.asString();
    int myInt = param.asInt();

    Serial.print("input V");
    Serial.print(request.pin);
    Serial.print(": ");
    Serial.println(myInt);
}
