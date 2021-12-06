#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// Set these to run example.
#define FIREBASE_HOST "arduino-e154b.firebaseio.com"
#define FIREBASE_AUTH "4O5e8uWo5s2TWrbrzrykH7ni1m5RPCTgh7SSA02X"
#define WIFI_SSID "0"
#define WIFI_PASSWORD "11111111"
void setup() {
Serial.begin(9600);
pinMode(D1, OUTPUT);
pinMode(D2, OUTPUT);
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
Firebase.set("LED_STATUS_D1", 0);
Firebase.set("LED_STATUS_D2", 0);

}
int mass[15];
//int d1=0;
//int d2=0;
mass[]
void loop() {
// get value
d1=Firebase.getInt("LED_STATUS_D1");
d2=Firebase.getInt("LED_STATUS_D2");

// handle error
if (d1==1) {
Serial.println("LED_D1 ON");
digitalWrite(D1,HIGH);  
//return;
delay(10);
}
else {
Serial.println("LED_D1 OFF");
digitalWrite(D1,LOW);  
}

if (d2==1) {
Serial.println("LED_D2 ON");
digitalWrite(D2,HIGH);
return;
delay(10);
}
else {
Serial.println("LED_D2 OFF");
digitalWrite(D2,LOW);  
return;
}

}
