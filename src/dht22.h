#include "Adafruit_Sensor.h"
#include "DHT.h"

// Выбираем тип нашего датчика (нужное раскоментировать)
// #define DHTTYPE DHT11     // DHT 11
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

const int DHTPin = 4; // выбираем GPIO для датчика

DHT dht(DHTPin, DHTTYPE); // создаем объект класса для работы с датчиком

void dht22_handle(int interval){ 
    static unsigned long i;
    static unsigned int in;
    if(i + (in * 1000) > millis() || interval == 0) return; // если не пришло время, или интервал = 0 - выходим из функции
    i = millis();
    in = interval;
    // всё, что ниже будет выполняться через интервалы

    static bool s = false;
    if(!s) { // это я сделал, чтобы не писать лишнее в setup()
        s = true;
        dht.begin(); // инициализируем библиотеку для работы с датчиком
    }

    float _tem = dht.readTemperature(); // получаем температуру
    float _hum= dht.readHumidity(); // получаем влажность
    

    if (isnan(_hum) || isnan(_tem)) { // проверяем, существуют ли данные о температуре и влажности в переменных (являются ли цыфрами)
		Serial.println("Не удалось прочитать данные от датчика DHT");
	}
	else{ // если данные с датчиков есть, выполняем всё что ниже
		// т.к. они для отладочных целей:
		Serial.println("Hum: " + String(_hum, 1) + "% Tem: " + String(_tem, 1) + "*C");  //  "Влажность: "
        // записываем полученные данные в глобальные переменные
		tem = _tem; // температура
		hum = _hum; // влажность
	}
}