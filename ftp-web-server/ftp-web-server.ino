#include <ESP8266WiFi.h>                                                // Библиотека для создания Wi-Fi подключения (клиент или точка доступа)
#include <ESP8266WebServer.h>                                           // Библиотека для управления устройством по HTTP (например из браузера)
#include <FS.h>                                                         // Библиотека для работы с файловой системой
#include <ESP8266FtpServer.h>                                           // Библиотека для работы с SPIFFS по FTP
//BME280
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

 
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)
#define ONE_WIRE_BUS 0

Adafruit_BME280 bme; // I2C



const byte relay = 2;                                                   // Пин подключения сигнального контакта реле
const char* ssid = "IIVA";
const char* password = "30092015";
 


ESP8266WebServer HTTP(80);                                              // Определяем объект и порт сервера для работы с HTTP
FtpServer ftpSrv;                                                       // Определяем объект для работы с модулем по FTP (для отладки HTML)
//========================================================================================
void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address ESP: ");
  Serial.println(WiFi.localIP());
 
  
  pinMode(relay,OUTPUT);                                                // Определяем пин реле как исходящий
  digitalWrite(relay, HIGH);
  /////FTP Setup, ensure SPIFFS is started before ftp;  /////////
SPIFFS.begin();                                                       // Инициализируем работу с файловой системой                          
  HTTP.begin();                                                         // Инициализируем Web-сервер
  
      Serial.println("SPIFFS opened! solist");
      ftpSrv.begin("solist","solist");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
  

// Обработка HTTP-запросов
  HTTP.on("/relay_switch", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_switch
      HTTP.send(200, "text/plain", relay_switch());                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_switch 
  });
  HTTP.on("/relay_status", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_status
      HTTP.send(200, "text/plain", relay_status());                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_status 
  });
    HTTP.on("/relay_switch_timer", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_switch
      HTTP.send(200, "text/plain", relay_switch());                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_switch 
  });
  HTTP.on("/relay_status_timer", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_status
      HTTP.send(200, "text/plain", relay_status());                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_status 
  });
  HTTP.onNotFound([](){                                                 // Описываем действия при событии "Не найдено"
  if(!handleFileRead(HTTP.uri()))                                       // Если функция handleFileRead (описана ниже) возвращает значение false в ответ на поиск файла в файловой системе
      HTTP.send(404, "text/plain", "Not Found");                        // возвращаем на запрос текстовое сообщение "File isn't found" с кодом 404 (не найдено)
  });
      bool status;
          // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    unsigned long timing;
}
//========================================================================================
void loop() {
 //   printValues();
    HTTP.handleClient();                                                // Обработчик HTTP-событий (отлавливает HTTP-запросы к устройству и обрабатывает их в соответствии с выше описанным алгоритмом)
    ftpSrv.handleFTP();                                                 // Обработчик FTP-соединений  

}
//========================================================================================
void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
 
    Serial.print("Pressure = ");
 
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");
 
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
 
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
 
    Serial.println();
}
//========================================================================================



String relay_switch() {                                                 // Функция переключения реле 
  byte state;
  if (digitalRead(relay))                                               // Если на пине реле высокий уровень   
    state = 0;                                                          //  то запоминаем, что его надо поменять на низкий
  else                                                                  // иначе
      state = 1;                                                          //  запоминаем, что надо поменять на высокий
  digitalWrite(relay, state);                                           // меняем значение на пине подключения реле
  return String(state);                                                 // возвращаем результат, преобразовав число в строку
}

String relay_status() {                                                 // Функция для определения текущего статуса реле 
  byte state;
  if (digitalRead(relay))                                               // Если на пине реле высокий уровень   
    state = 1;                                                          //  то запоминаем его как единицу
  else                                                                  // иначе
    state = 0;                                                          //  запоминаем его как ноль
  return String(state);                                                 // возвращаем результат, преобразовав число в строку
}

String relay_switch_timer() {                                                 // Функция переключения реле 
  byte state;
  if (digitalRead(relay))                                               // Если на пине реле высокий уровень   
    state = 0;                                                          //  то запоминаем, что его надо поменять на низкий
  else                                                                  // иначе
      state = 1;                                                          //  запоминаем, что надо поменять на высокий
  if (millis() - timing > 10000){ // Вместо 10000 подставьте нужное вам значение паузы 
  timing = millis(); 
  Serial.println ("10 seconds");
 }
    
  digitalWrite(relay, state);                                           // меняем значение на пине подключения реле
  return String(state);                                                 // возвращаем результат, преобразовав число в строку
}

String relay_status_timer() {                                                 // Функция для определения текущего статуса реле 
  byte state;
  if (digitalRead(relay))                                               // Если на пине реле высокий уровень   
    state = 1;                                                          //  то запоминаем его как единицу
  else                                                                  // иначе
    state = 0;                                                          //  запоминаем его как ноль
  return String(state);                                                 // возвращаем результат, преобразовав число в строку
}

bool handleFileRead(String path){                                       // Функция работы с файловой системой
  if(path.endsWith("/")) path += "index.html";                          // Если устройство вызывается по корневому адресу, то должен вызываться файл index.html (добавляем его в конец адреса)
  String contentType = getContentType(path);                            // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
  if(SPIFFS.exists(path)){                                              // Если в файловой системе существует файл по адресу обращения
    File file = SPIFFS.open(path, "r");                                 //  Открываем файл для чтения
    size_t sent = HTTP.streamFile(file, contentType);                   //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
    file.close();                                                       //  Закрываем файл
    return true;                                                        //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
  }
  return false;                                                         // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}

String getContentType(String filename){                                 // Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла
  if (filename.endsWith(".html")) return "text/html";                   // Если файл заканчивается на ".html", то возвращаем заголовок "text/html" и завершаем выполнение функции
  else if (filename.endsWith(".css")) return "text/css";                // Если файл заканчивается на ".css", то возвращаем заголовок "text/css" и завершаем выполнение функции
  else if (filename.endsWith(".js")) return "application/javascript";   // Если файл заканчивается на ".js", то возвращаем заголовок "application/javascript" и завершаем выполнение функции
  else if (filename.endsWith(".png")) return "image/png";               // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
  else if (filename.endsWith(".jpg")) return "image/jpeg";              // Если файл заканчивается на ".jpg", то возвращаем заголовок "image/jpg" и завершаем выполнение функции
  else if (filename.endsWith(".gif")) return "image/gif";               // Если файл заканчивается на ".gif", то возвращаем заголовок "image/gif" и завершаем выполнение функции
  else if (filename.endsWith(".ico")) return "image/x-icon";            // Если файл заканчивается на ".ico", то возвращаем заголовок "image/x-icon" и завершаем выполнение функции
  return "text/plain";                                                  // Если ни один из типов файла не совпал, то считаем что содержимое файла текстовое, отдаем соответствующий заголовок и завершаем выполнение функции
}
