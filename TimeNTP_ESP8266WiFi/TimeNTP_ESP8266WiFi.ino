#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FirebaseArduino.h>
#include "DHTesp.h"
#define FIREBASE_HOST "arduino-e154b.firebaseio.com"
#define FIREBASE_AUTH "4O5e8uWo5s2TWrbrzrykH7ni1m5RPCTgh7SSA02X"
#define rmp1 12
#define rmp2 13
#define rmp3 14
#define rmp4 15
DHTesp dht;

//const char ssid[] = "IVA";  //  your network SSID (name)
//const char pass[] = "30092015";       // your network password
const char ssid[] = "0";  //  your network SSID (name)
const char pass[] = "11111111";       // your network password

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
const int timeZone = 3;     // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

unsigned long last_time;
unsigned long last_temp;

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
  Serial.println("TimeNTP Example");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  
  dht.setup(16, DHTesp::DHT22);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.set("TMP", 0);
  //Firebase.set("HUM", 0);
  //Firebase.set("rm1", 0);
  //Firebase.set("rm2", 0);
  //Firebase.set("rm3", 0);
  Firebase.set("rm4", 0);
  
  
  Serial.println(last_time);
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
/*  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }
  */
  if (millis() - last_time > 10000){
  last_time = millis();
  hms();
  rm();
    Serial.println(last_time);

  }
  if (millis() - last_temp > 30000){
  last_temp = millis();
  fdht();
  }
 }

void fdht()
{
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

    Serial.println(str_t);
   Firebase.setFloat(str_t,t);

    Serial.println(str_h);
   Firebase.setFloat(str_h,h);

        
  }
  
void rm()
{

  int f1 = Firebase.getInt("rm1");
  int f2 = Firebase.getInt("rm2");
  int f3 = Firebase.getInt("rm3");
  int f4 = Firebase.getInt("rm4");

  if (f1 == 1){
    digitalWrite(rmp1, LOW);
    //Serial.println('1_1');
    }
   else if (f1 == 0){
    digitalWrite(rmp1, HIGH);
    //Serial.println('1_0');
    }
  if (f2 == 1){
    digitalWrite(rmp2, LOW);
    //Serial.println('2_1');
    }
   else if (f2 == 0){
    digitalWrite(rmp2, HIGH);
    //Serial.println('2_0');
    }
   
  if (f3 == 1){
    digitalWrite(rmp3, LOW);
    //Serial.println('3_1');
    }
   else if (f3 == 0){
    digitalWrite(rmp3, HIGH);
    //Serial.println('3_0');
    }
   
  if (f4 == 1){
    digitalWrite(rmp4, LOW);
    }
   else if (f4 == 0){
    digitalWrite(rmp4, HIGH);
    }
   
   
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
