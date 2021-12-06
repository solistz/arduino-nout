#include <Wire.h>

#include <SI7021.h>

 

SI7021 sensor;

int temp1,hum1;

 

void setup() {

 Serial.begin(9600);

 sensor.begin();

}

 

void loop() {

 delay(2000);

 

 si7021_env data = sensor.getHumidityAndTemperature();

 temp1=data.celsiusHundredths-data.celsiusHundredths/100*100;

 hum1=data.humidityBasisPoints-data.humidityBasisPoints/100*100;

  

 Serial.print("temperatura ");

      Serial.print(data.celsiusHundredths/100);

      Serial.print(",");

      Serial.print(temp1);

      Serial.println(" `C");

 

 Serial.print("Humidity ");

      Serial.print(data.humidityBasisPoints/100);

      Serial.print(",");

      Serial.print(hum1);

      Serial.println(" \%");

      Serial.println(" ");

}
