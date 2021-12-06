#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "DHT.h"       // подключаем библиотеку для датчика

#define TRIGGER 5
#define ECHO    4

// Set these to run example.
#define FIREBASE_HOST "arduino-e154b.firebaseio.com"
#define FIREBASE_AUTH "4O5e8uWo5s2TWrbrzrykH7ni1m5RPCTgh7SSA02X"
//#define WIFI_SSID "KniazIphone"
//#define WIFI_PASSWORD "sst66zneouy8d"
#define WIFI_SSID "IVA"
#define WIFI_PASSWORD "30092015"

DHT dht(16, DHT11); // сообщаем на каком порту будет датчик


void setup() {
pinMode(TRIGGER, OUTPUT);
pinMode(ECHO, INPUT);

dht.begin();

Serial.begin(9600);
pinMode(D3, OUTPUT);
pinMode(D4, OUTPUT);
pinMode(D5, OUTPUT);
pinMode(D6, OUTPUT);
pinMode(D7, OUTPUT);
pinMode(D8, OUTPUT);

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
Firebase.set("LDR", 0);
Firebase.set("LDL", 0);
Firebase.set("LF", 0);
Firebase.set("RG", 0);
Firebase.set("DR", 0);
Firebase.set("RV", 0);

}

int d1a;
int d2a;
int lfa;
int rga;
int dra;
int rva;

int a = 0;
int b = 0;

void loop() {
// get value



int d1=0;
int d2=0;
int lf=0;
int rg=0;
int dr=0;
int rv=0;

d1=Firebase.getInt("LDR");
d2=Firebase.getInt("LDL");

lf=Firebase.getInt("LF");
rg=Firebase.getInt("RG");
dr=Firebase.getInt("DR");
rv=Firebase.getInt("RV");

Serial.println(d1);
Serial.println(d2);

Serial.println(lf);
Serial.println(rg);
Serial.println(dr);
Serial.println(rv);

// handle error
if (d1!=d1a) {
Serial.println("LDR ON");
digitalWrite(D3, d1); 
d1a = d1; 
}

if (d2!=d2a) {
Serial.println("LDL ON");
digitalWrite(D4, d2); 
d2a = d2; 
}

if (lf!=lfa) {
Serial.println("LF ON");
digitalWrite(D5, lf); 
lfa = lf; 
}

if (rg!=rga) {
Serial.println("RG ON");
digitalWrite(D6, rg); 
rga = rg; 
}

if (dr!=dra) {
Serial.println("DR ON");
digitalWrite(D7, dr); 
dra = dr; 
}

if (rv!=rva) {
Serial.println("RV ON");
digitalWrite(D8, rv); 
rva = rv;
}


if (a == 20)
{
  Serial.println("aaaaa");
   float h = dht.readHumidity();
   float t = dht.readTemperature();
   Firebase.set("TEMP", t);
   Firebase.set("HUM", h);
  a=0;
  }

  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(7); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  Firebase.set("DIST", distance);
  
  
  Serial.println(distance);
 
delay(25);
a++;
b++;
}
