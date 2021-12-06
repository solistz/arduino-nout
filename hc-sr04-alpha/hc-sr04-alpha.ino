#define trigPin 5

#define echoPin 4

#define led 00

#define led2 2

void setup() {

Serial.begin (9600);

pinMode(trigPin, OUTPUT);

pinMode(echoPin, INPUT);

pinMode(led, OUTPUT);

pinMode(led2, OUTPUT);

}

void loop() {

long duration, distance;

digitalWrite(trigPin, LOW);

delayMicroseconds(2);

digitalWrite(trigPin, HIGH);

delayMicroseconds(10);

digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH);

distance = (duration/2) / 29.1;

if (distance < 20) { // На этом этапе происходит вкл/выкл светодиода digitalWrite(led,HIGH); // когда загорается красный, зеленый обязан выключится

digitalWrite(led2,LOW); } else { digitalWrite(led,LOW); digitalWrite(led2,HIGH); } if (distance >= 200 || distance <= 0){

Serial.println("Out of range"); // Можно придумать что-то на русском – вроде "Вне диапазона"

}

else {

Serial.print(distance);

Serial.println(" cm"); // тут тоже можно указать " см"

}

delay(500);

}
