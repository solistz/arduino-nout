/* 
Тестировалось на Arduino IDE 1.8.5
Дата тестирования 28.03.2019г.
*/ 
 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);          // Выводы SIM800L Tx & Rx подключены к выводам Arduino 3 и 2

void setup()
{
  Serial.begin(115200);                   // Инициализация последовательной связи с Arduino и Arduino IDE (Serial Monitor)
  mySerial.begin(115200);                 // Инициализация последовательной связи с Arduino и SIM800L
  Serial.println("Initializing...");    // Печать текста
  delay(1000);                          // Пауза 1 с

  mySerial.println("AT");               // Отправка команды AT
  updateSerial();
  mySerial.println("AT+CSQ");           // Проверка качества сигнала, диапазон значений 0-31, 31 - лучший
  updateSerial();
  mySerial.println("AT+CCID");          // Чтение информацию о SIM-карте
  updateSerial();
  mySerial.println("AT+CREG?");         // Проверка регистрации в сети
  updateSerial();
}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  delay(500);                           // Пауза 500 мс
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());      // Переадресация с последовательного порта SIM800L на последовательный порт Arduino IDE
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());      // Переадресация c Arduino IDE на последовательный порт SIM800L
  }
}
