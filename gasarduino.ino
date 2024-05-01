#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 5
#define MQ2PIN A0
#define MQ2D2PIN 8  // Digital pin for MQ2 sensor
#define MQ9PIN A2
#define MQ9D2PIN 10  // Digital pin for MQ9 sensor
#define DHTTYPE DHT11

SoftwareSerial espSerial(3, 2);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Assuming a 16x2 LCD

float temperature, humidity;
int mq2Analog, mq9Analog, mq2Digital, mq9Digital;

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(MQ2D2PIN, INPUT);
  pinMode(MQ9D2PIN, INPUT);
}

void loop() {
  delay(2000);
  readDHTData();
  readMQ2Data();
  readMQ9Data();
  updateLCD();
  sendToESP();
  delay(2000);
}

void readDHTData() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("C  Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }
}


void readMQ2Data() {
  mq2Analog = analogRead(MQ2PIN);
  mq2Digital = digitalRead(MQ2D2PIN);
}

void readMQ9Data() {
  mq9Analog = analogRead(MQ9PIN);
  mq9Digital = digitalRead(MQ9D2PIN);
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C   ");

  // lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print(" %");

  lcd.setCursor(0, 2);
  lcd.print("MQ2: ");
  lcd.print(mq2Analog);
  lcd.print(" (");
  lcd.print(mq2Digital);
  lcd.print(")");

  lcd.setCursor(0, 3);
  lcd.print("MQ9: ");
  lcd.print(mq9Analog);
  lcd.print(" (");
  lcd.print(mq9Digital);
  lcd.print(")");
}


void sendToESP() {
  String postData = "temperature=" + String(temperature) +
                    "&humidity=" + String(humidity) +
                    "&mq2_analog=" + String(mq2Analog) +
                    "&mq2_digital=" + String(mq2Digital) +
                    "&mq9_analog=" + String(mq9Analog) +
                    "&mq9_digital=" + String(mq9Digital);
  Serial.println("Sending data to ESP: " + postData);
  espSerial.println(postData);
  while (espSerial.available() > 0) {
    char responseChar = espSerial.read();
    Serial.print(responseChar);
  }
}
