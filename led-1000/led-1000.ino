void setup() {
  // выставляем 13-ый цифровой контакт в режим вывода данных:
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(115200);
}

// блок loop() будет работать снова и снова:
void loop() {
  digitalWrite(13, HIGH);   // включаем светодиод 
                           // (т.е. подаем значение HIGH)
  delay(1000);             // ждем секунду
  digitalWrite(2, HIGH);   // включаем светодиод 
                           // (т.е. подаем значение HIGH)
  delay(1000);             // ждем секунду
  Serial.println("TimeNTP Example WWWWWWWWWW");
  digitalWrite(13, LOW);    // выключаем светодиод
                           // (т.е. подаем значение LOW)
  delay(1000);             // ждем секунду
  Serial.println("TimeNTP Example WWWWWWWWWW");
  digitalWrite(2, LOW);    // выключаем светодиод
                           // (т.е. подаем значение LOW)
  delay(1000);             // ждем секунду

}
