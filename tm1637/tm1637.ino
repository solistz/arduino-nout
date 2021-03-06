int8_t DispMSG[] = {1, 2, 3, 4};   // Настройка символов для последующего вывода на дислей
//Определяем пины для подключения к плате Arduino
#define CLK 3
#define DIO 2
//Создаём объект класса TM1637, в качестве параметров
//передаём номера пинов подключения
TM1637 tm1637(CLK, DIO);
void setup()
{
  //Инициализация модуля
  tm1637.init();
  //Установка яркости горения сегментов
  /*
     BRIGHT_TYPICAL = 2 Средний
     BRIGHT_DARKEST = 0 Тёмный
     BRIGHTEST = 7      Яркий
  */
  tm1637.set(BRIGHT_TYPICAL);
}
void loop()
{
  //Задание на включение разделителя  
  tm1637.point(true);
  //Выводим массив на дисплей  
  tm1637.display(DispMSG);
  //Задержка
  delay(1000);
  //Задание на выключение разделителя
  tm1637.point(false);
  //Выводим массив на дисплей
  tm1637.display(DispMSG);
  //Задержка
  delay(1000);
}
