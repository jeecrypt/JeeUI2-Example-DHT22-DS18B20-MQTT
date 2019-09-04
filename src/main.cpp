



#include <Arduino.h>
#include "JeeUI2.h" // Подключаем JeeUI2 фреймворк к проекту
jeeui2 jee; // Создаем объект класса для работы с JeeUI2 фреймворком

#include "interface.h"  // в этот файл вынесена работа с параметрами и с веб интерфейсом
#include "dht22.h"      // в этом файле работа с датчиком DHT22
#include "relay.h"      // в этом файле работа с реле
#include "ds18b20.h"    // в этом файле работа с датчиком ds18b20

void mqttCallback(String topic, String payload);
void sendData();

void setup() {
  Serial.begin(115200);
  jee.mqtt("m21.cloudmqtt.com", 15486, "iukuegvk", "gwo8tlzvGJrR", mqttCallback, true);
  jee.udp(jee.mc); // Ответ на UDP запрс. в качестве аргуиена - переменная, содержащая id по умолчанию
  jee.led(2, false); // назначаем пин на светодиод, который нам будет говорит о состоянии устройства. (быстро мигает - пытается подключиться к точке доступа, просто горит (или не горит) - подключен к точке доступа, мигает нормально - запущена своя точка доступа)
  jee.ap(20000); // если в течении 20 секунд не удастся подключиться к Точке доступа - запускаем свою (параметр "wifi" сменится с AP на STA)
  parameters(); // создаем параметры
  jee.ui(interface); // обратный вызов - интерфейс
  jee.update(update); // обратный вызов - вызывается при введении данных в веб интерфейс, нужна для сравнения значений пременных с параметрами
  jee.begin(true); // Инициализируем JeeUI2 фреймворк. Параметр bool определяет, показывать ли логи работы JeeUI2 (дебаг)
  update(); // 'этой функцией получаем значения параметров в переменные
}

void loop() {
  jee.handle(); // цикл, необходимый фреймворку
  sendData(); // цикл отправки данных по MQTT

  // оставьте только дну из функций ниже, если у вас только одни датчик
  ds_handle(ds_int); // цикл замера температуры ds18b20
  dht22_handle(dht_int); //функция, периодически получающая параметров с датчика
}

void mqttCallback(String topic, String payload){ // функция вызывается, когда приходят данные MQTT
  Serial.println("Message [" + topic + " - " + payload + "] ");

}

void sendData(){
  static unsigned long i;
  static unsigned int in;
  if(i + (in * 1000) > millis() || dht_int == 0) return; // если не пришло время, или интервал = 0 - выходим из функции
  i = millis();
  in = mqtt_int;
  // всё, что ниже будет выполняться через интервалы

  jee.publish("tem", String(tem));
  jee.publish("hum", String(hum));
  jee.publish("ds18b20", String(ds_tem));

}