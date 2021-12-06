#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// Set these to run example.
#define FIREBASE_HOST "test-fd331.firebaseio.com"
#define FIREBASE_AUTH "z7QmrqavNDt1jBFrkeTtqzqLzdYENPNydOkf8ODY"
#define WIFI_SSID "IVA"
#define WIFI_PASSWORD "30092015"
void setup() {
Serial.begin(9600);
pinMode(D1, OUTPUT);
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
Serial.print("firebase db: ");
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.set("LED_STATUS", 0);
}
int n = 0;
void loop() {

if (Firebase.failed())
    {
//пытаемся подключиться к FireBase, но в моём случае не помогает(
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      delay(1000);
      Serial.print("errors db: ");
      return;
    }
    //если ошибок нет
    else
    {
//тут можно что-то покодить для мониторинга положительного результата подключения
// get value
n = Firebase.getInt("LED_STATUS");
// handle error
if (n==1) {
Serial.println("LED ON");
digitalWrite(D1,HIGH);  
return;
delay(10);
}
else {
Serial.println("LED OFF");
digitalWrite(D1,LOW);  
return;
}
    }
    delay(10);
}
