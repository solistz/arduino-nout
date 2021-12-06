#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
#define DHTTYPE DHT22
#define dht_dpin 2
#define ONE_WIRE_BUS 0
#define smokePin A0
#define RM46 16
#define RM44 14
#define RM43 13
#define RM42 12
#define pirPin 15

Adafruit_BME280 bme; // I2C
int sensorThres = 400;
DHT dht(dht_dpin, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(RM46, OUTPUT);
  pinMode(RM44, OUTPUT);
  pinMode(RM43, OUTPUT);
  pinMode(RM42, OUTPUT);

  //dht.begin();
  sensors.begin();
  Serial.begin(9600);
  Serial.println(F("BME280 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  //digitalWrite(RM46, HIGH);
  //digitalWrite(RM44, HIGH);
  //digitalWrite(RM42, HIGH);
  //digitalWrite(RM43, HIGH);

  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.print("C  ");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.print("%  ");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.print("hPa   ");

  Serial.print("Approx altitude = ");
  Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
  Serial.println("m   ");

  //===========================================================================
  //delay(1000);
  //===========================================================================
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("C   ");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println("%   ");
  //===========================================================================
  //delay(1000);
  //===========================================================================

  Serial.print("Temperature = ");
  Serial.println(sensors.getTempCByIndex(0));

  //===========================================================================
  //delay(1000);
  //===========================================================================

  int analogSensor = analogRead(smokePin);    // считываем значения АЦП с аналогового входа // к которому подключен датчик
  Serial.print (analogSensor);                // выводим в порт значение АЦП сигнала с датчика

  // Проверяем, достигнуто ли пороговое значение
  if (analogSensor > sensorThres) {  // если значение больше допустимого...
    Serial.println(" Gaz!");         // выводим в порт надпись, что газ есть
    for (int i = 0; i < 10; i++) {

      digitalWrite(RM43, LOW);
      digitalWrite(RM42, LOW);
      delay(500);
      digitalWrite(RM43, HIGH);
      digitalWrite(RM46, HIGH);
      delay(500);

    }
  }
  Serial.println();

  //===========================================================================
  delay(1000);
  //===========================================================================

  int pirVal = digitalRead(pirPin); //Считываем значения с датчика движения. Если обнаружили движение,
  //то транслируем сигнал тревоги в монитор порта и включаем светодиод
  if (pirVal == HIGH)
  {
    Serial.print("Тревога ");
    for (int i = 0; i < 10; i++) {

      digitalWrite(RM44, LOW);
      digitalWrite(RM46, LOW);
      delay(500);
      digitalWrite(RM44, HIGH);
      digitalWrite(RM46, HIGH);
      delay(500);

    }

    delay(1000);
  }
  else
  {
    Serial.print("Сканирую ");
    //  digitalWrite(14, HIGH);
    //    digitalWrite(16, HIGH);

    delay(1000);
  }



}
