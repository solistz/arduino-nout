#define pirPin 14
#define LedPin 2
  
void setup() {
  Serial.begin(115200); // Объявляем работу com порта со скоростью 9600
  pinMode(pirPin, INPUT); //Объявляем пин, к которому подключен датчик движения, входом
  pinMode(LedPin, OUTPUT); //Объявляем пин, к которому подключен светодиод, выходом
}
  
void loop() {
  int pirVal = digitalRead(pirPin); //Считываем значения с датчика движения. Если обнаружили движение, 
                                    //то транслируем сигнал тревоги в монитор порта и включаем светодиод
  if(pirVal == LOW)
  {
    digitalWrite(LedPin, HIGH);
    Serial.println("Тревога ");
    delay(2000);
  }
else
{
  Serial.println("Сканирую ");
  digitalWrite(LedPin,LOW);
  delay(1000);
}
}
