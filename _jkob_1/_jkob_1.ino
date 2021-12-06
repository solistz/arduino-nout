//NTP
#include <TimeLib.h>
//Wi-Fi
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//FB
#include "FirebaseESP8266.h"
#define FIREBASE_HOST "arduino-e154b.firebaseio.com"
#define FIREBASE_AUTH "4O5e8uWo5s2TWrbrzrykH7ni1m5RPCTgh7SSA02X"


//DHT22
#include "DHTesp.h"

#include <Wire.h>
#include <SPI.h>
//BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//temp
#include <OneWire.h>
#include <DallasTemperature.h>

#define rmp1 12
#define rmp2 13
#define rmp3 14
#define rmp4 3

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
#define ONE_WIRE_BUS 0

#define smokePin A0    // определяем аналоговый выход к которому подключен датчик 
int sensorThres = 400; // пороговое значение АЦП, при котором считаем что газ есть 
float temp280 = 0;

DHTesp dht;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char ssid[] = "IIVA";  //  your network SSID (name)
const char pass[] = "30092015";       // your network password
//const char ssid[] = "kz";  //  your network SSID (name)
//const char pass[] = "1111111111";       // your network password
//const char ssid[] = "iiva";  //  your network SSID (name)
//const char pass[] = "3009201517042019";       // your network password

// NTP Servers:
static const char ntpServerName[] = "0.europe.pool.ntp.org";
const int timeZone = 3;     // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

unsigned long last_time;
unsigned long last_temp;

Adafruit_BME280 bme; // I2C
FirebaseData firebaseData;


void setup()
{
  
  pinMode(rmp1, OUTPUT);
  pinMode(rmp2, OUTPUT);
  pinMode(rmp3, OUTPUT);
  pinMode(rmp4, OUTPUT);
  digitalWrite(rmp1, HIGH);
  digitalWrite(rmp2, HIGH);
  digitalWrite(rmp3, HIGH);
  digitalWrite(rmp4, HIGH);

  Serial.begin(9600);
  delay(250);
  Serial.println("TimeNTP Example WWWWWWWWWW");

  wf();
  
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  
  dht.setup(16, DHTesp::DHT22);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
    bool status;
    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        //while (1);
    }
    Serial.println("-- Default Test --");
    Serial.println();
//    delay(100); 


  
  //Serial.println(last_time);

 sensors.begin();
 
 }

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
//  if (millis() - last_time > 10000){
//  last_time = millis();
//}
  if (millis() - last_temp > 10000){
  last_temp = millis();
  Serial.println(last_temp);
  hms();
  fdht();
  bmp280();
  ds18b20();
  mq2();
  }

      
   
 }

void wf(){
    Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  }

void mq2(){
  Serial.println("######################################### MQ2"); 
    int analogSensor = analogRead(smokePin);    // считываем значения АЦП с аналогового входа 
                                              // к которому подключен датчик  
  Serial.print (analogSensor);                // выводим в порт значение АЦП сигнала с датчика 
   
  // Проверяем, достигнуто ли пороговое значение 
  if (analogSensor > sensorThres) {  // если значение больше допустимого... 
    Serial.println(" Gaz! - SOS");         // выводим в порт надпись, что газ есть 
  } 
  else {                             // иначе... 
   Serial.println(" normal");        // выводим в порт надпись, что газа нет 
  } 
  }

void ds18b20(){
  Serial.println("######################################### DS18B"); 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  //Serial.print("Requesting temperatures...");
  //sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature : ");
  Serial.println(sensors.getTempCByIndex(0));
  }

void bmp280() {
  Serial.println("######################################### BMP280"); 
    Serial.print("Temperature = ");
    Serial.print("\t");
    Serial.print(bme.readTemperature());
    Serial.print(" *C");
 Serial.print("\t");
    Serial.print("Pressure = ");
     Serial.print(bme.readPressure() / 100.0F);
    Serial.print(" hPa");
Serial.print("\t"); 
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print(" m");
 Serial.print("\t");
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
  Serial.println("\t");

        Serial.println(temp280);
delay(3000);
        int hum280 = bme.readHumidity();
if (hour() >=0 & hour() <=9 ){
    String hours = "0"+String(hour());
    int result = hours.toInt();
      Serial.println(result);

}
    String str_t1 ="/BMP280/" + String(day()) + "/" + "_" + String(minute()) +"/"+"T";
    String str_h1 ="BMP280/" + String(day()) + "/" +  String(hour()) + "_" + String(minute()) +"/"+"H";
if ((temp280+0.5) <= bme.readTemperature() || (temp280-0.5) >= bme.readTemperature()){
  temp280 = bme.readTemperature();
  Firebase.setInt(firebaseData, str_t1, temp280);
  Serial.println("\t");
      Serial.println(" WORK IF");

     }else{
  Serial.println("\t");
      Serial.println(" NOT IF");  
//      Serial.println(result());  

}
Firebase.setInt(firebaseData, "/humd", hum280);



//   Firebase.setFloat(str_t1,temp280);
//   Firebase.setFloat(str_h1,hum280);
}

void fdht()
{
  Serial.println("######################################### DHT22"); 
  delay(dht.getMinimumSamplingPeriod());
  float h = dht.getHumidity();
  float t = dht.getTemperature();
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(h, 1);
  Serial.print("\t\t");
  Serial.print(t, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(t), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(t, h, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(t), h, true), 1);
  //Firebase.setFloat("2019/dht/" + minute(),temperature);
//  Firebase.set("2019/04/",humidity);
//Serial.println(minute());


      
    String str_t =String(year()) + "/" + String(month()) + "/" + String(day()) + "/" +  String(hour()) + "_" + String(minute()) +"/"+"T";
    String str_h =String(year()) + "/" + String(month()) + "/" + String(day()) + "/" +  String(hour()) + "_" + String(minute()) +"/"+"H";

    //Serial.println(str_t);
//   Firebase.setFloat(str_t,t);

    //Serial.println(str_h);
//   Firebase.setFloat(str_h,h);

        
  }
  


void hms()
{
  Serial.print(hour());
  Serial.print(" ");
  Serial.print(minute());
  Serial.print(" ");
  Serial.print(second());
  Serial.println(" ");
  }

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  Serial.print(minute());
  Serial.print(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();

}


/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
