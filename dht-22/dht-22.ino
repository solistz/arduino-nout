#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "DHT.h"        // including the library of DHT22 temperature and humidity sensor

// Set these to run example.
#define FIREBASE_HOST "arduino-e154b.firebaseio.com"
#define FIREBASE_AUTH "4O5e8uWo5s2TWrbrzrykH7ni1m5RPCTgh7SSA02X"
#define WIFI_SSID "IVA"
#define WIFI_PASSWORD "30092015"

#define DHTTYPE DHT22   // DHT 22
#define dht_dpin 10
DHT dht(dht_dpin, DHTTYPE); 
void setup(void)
{
  dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature\n\n");
  delay(5000);

  // connect to wifi.
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.set("TMP", 0);
Firebase.set("HUM", 0);
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();   
    Firebase.set("TEMP", t);      
    Firebase.set("HUMID", h);      
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    
  delay(3000);
}
