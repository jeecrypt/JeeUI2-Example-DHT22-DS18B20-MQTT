#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 13

float ds_tem;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void ds_setup(void)
{
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 

  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
}

void getTemperature(DeviceAddress deviceAddress)
{
  ds_tem = sensors.getTempC(deviceAddress);
}

void ds_handle(int interval)
{ 
    static unsigned long i;
    static unsigned int in;
    if(i + (in * 1000) > millis() || interval == 0) return; // если не пришло время, или интервал = 0 - выходим из функции
    i = millis();
    in = interval;
    // всё, что ниже будет выполняться через интервалы

    static bool st = false;
    if(!st){
        st = true;
        ds_setup();
    }
  sensors.requestTemperatures();
  getTemperature(insideThermometer); 
  Serial.println("Temperature: " + String(ds_tem, 2));
}

