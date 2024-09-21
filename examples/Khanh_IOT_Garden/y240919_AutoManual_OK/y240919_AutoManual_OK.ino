// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "TMPL6koTRhUeq"
#define BLYNK_TEMPLATE_NAME "IOT Garden"
#define BLYNK_AUTH_TOKEN "mfBIlnpKTwX0XwB0rTTvm35CwzzMUzw9"

// Step 2: include library
#include "BlynkGate.h"
#include "MKL_LiquidCrystal_I2C.h"
#include "MKL_DHT.h"
// #include "MKL_OneButton.h"
#include "MAKERLABVN.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

#define LDR_MIN_VALUE 20
#define LDR_MAX_VALUE 679

#define LDR_PIN A3
#define DHTPIN A1  // Digital pin connected to the DHT sensor

#define analogInPin A2

MKL_DHT dht(DHTPIN, DHT11);

void readLDR();
void showOnLCD();
void readDHT11();
void Bump_control();
void Auto_Bump();

unsigned long intervalLCD = 0;
unsigned long lastTimeSend = 0;
MKL_LiquidCrystal_I2C lcd(0x27, 16, 2);

bool on = 0;
bool automatic = true;
int sensorValue = 0;
float sensorValue1 = 0;
float sensorValue2 = 0;
int Virtual_Pins_Bump = 4;
int valueLDR = 0;
int lightPercent = 0;
int bumpState = 1;
int humidityPercen = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Start BlynkGate I2C"));
  lcd.init();
  lcd.backlight();
  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(10, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  readLDR();
  readDHT11();
  readSMoisture();
  Bump_control();
  Auto_Bump();
  if (millis() - intervalLCD > 500) {
    lcd.clear();
    showOnLCD();
    intervalLCD = millis();
  }
  unsigned long intervalBump = 0;
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSend >= 10000) {
    lastTimeSend = millis();
    if (millis() - intervalBump >= 1000) {
      intervalBump = millis();
      if (automatic)
        Blynk.virtualWrite(4, !on);
    }
    Blynk.virtualWrite(0, lightPercent);
    Blynk.virtualWrite(1, sensorValue1);
    Blynk.virtualWrite(2, sensorValue2);
    Blynk.virtualWrite(3, sensorValue);


    // Step 6: Send Virtual pin Value
  }
}
int paMyIntLast = 0;
int paMyInt = 0;
// Step 5: Get Virtual pin Value
BLYNK_WRITE_DEFAULT() {
  // double myDouble = param.asFloat();
  // String myString = param.asString();
  int myInt = param.asInt();

  Serial.print("input V");
  Serial.print(request.pin);
  Serial.print(": ");
  Serial.println(myInt);
  if (request.pin == Virtual_Pins_Bump) {
    if (myInt == 1) {  // Nếu giá trị nhận được là 1
      bumpState = 0;
      paMyInt = myInt;
    } else {  // Nếu giá trị nhận được không phải là 1
      bumpState = 1;
      paMyInt = 0;
    }
  }
}

void readLDR() {
  valueLDR = analogRead(LDR_PIN);
  lightPercent = map(valueLDR, LDR_MAX_VALUE, LDR_MIN_VALUE, 0, 100);
}

void showOnLCD() {
  lcd.setCursor(1, 0);
  lcd.print("T:");
  lcd.setCursor(4, 0);
  lcd.print((int)sensorValue1);

  lcd.setCursor(9, 0);
  lcd.print("H:");
  lcd.setCursor(12, 0);
  lcd.print((int)sensorValue2);

  lcd.setCursor(1, 1);
  lcd.print("S:");
  lcd.setCursor(4, 1);
  lcd.print((int)sensorValue);

  lcd.setCursor(9, 1);
  lcd.print("L:");
  lcd.setCursor(12, 1);
  lcd.print((int)lightPercent);

  if ((int)sensorValue1 / 100 < 1) {
    lcd.setCursor(6, 0);
    lcd.print('C');
  } else {
    lcd.setCursor(7, 0);
    lcd.print('C');
  }

  if ((int)sensorValue2 / 100 < 1) {
    lcd.setCursor(14, 0);
    lcd.print('H');
  } else {
    lcd.setCursor(15, 0);
    lcd.print('H');
  }

  if ((int)sensorValue / 100 < 1) {
    lcd.setCursor(6, 1);
    lcd.print('%');
  } else {
    lcd.setCursor(7, 1);
    lcd.print('%');
  }

  if ((int)lightPercent / 100 < 1) {
    lcd.setCursor(14, 1);
    lcd.print('%');
  } else {
    lcd.setCursor(15, 1);
    lcd.print('%');
  }
}

void readDHT11() {
  sensorValue1 = dht.readTemperature();
  sensorValue2 = dht.readHumidity();
}

void readSMoisture() {
  sensorValue = map(analogRead(analogInPin), 676, 0, 0, 100);
}

unsigned long lastMillis = 0;

void Bump_control() {
  if (paMyInt != paMyIntLast) {
    lastMillis = millis();
    paMyIntLast = paMyInt;
    automatic = false;
    digitalWrite(10, bumpState);
  } else if (!automatic) {
    if (millis() - lastMillis >= 10000) {
      Serial.println("Start Automatic");
      automatic = true;
    }
  }
}


void Auto_Bump() {
  if (automatic) {
    if (sensorValue < 30) {
      on = false;
      digitalWrite(10, on);
    } else if (sensorValue > 70)
      on = true;
    digitalWrite(10, on);
  }
}