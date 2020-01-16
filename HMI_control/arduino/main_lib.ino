#include <DHT.h>  //depend on Adafruit_Sensor, didn't install yet
#include <DS3231.h>
#include <DallasTemperature.h>
#include <GUVA-S12SD.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#define RELAY_PIN 2
#define ONE_WIRE_PIN 3
#define DHT_PIN 4
#define DHTTYPE DHT11

DS3231 Clock;
bool setPM = false;  //set it to false
bool Century = false;
bool had_been_irrigated = false;
int morning;
int night;
unsigned long irrigateDuration;
SoftwareSerial BT(7, 6);                 //arduino RX/TX
GUVAS12SD uvSensor(A0);                  //UVs
OneWire oneWire(ONE_WIRE_BUS);           //wire of temp
DallasTemperature tempSensor(&oneWire);  //temperature
DHT dhtSensor(DHT_PIN, DHTTYPE);

void setTime() {
    Clock.setYear(20);
    Clock.setMonth(1);
    Clock.setDate(12);
    Clock.setDoW(0);
    Clock.setHour(10);  //the format is 24 hours
    Clock.setMinute(27);
    Clock.setSecond(0);
}

String getTime() {
    return String(Clock.getMonth(Century)) + String("-") +
           String(Clock.getDate()) + String("-") +
           String(Clock.getYear()) + String(" ") +
           String((int)Clock.getHour(false, setPM)) + String(":") +
           String(Clock.getMinute()) + String(":") +
           String(Clock.getSecond());
}

int getUV() {
    return (int)uvSensor.index(uvSensor.read()) + 1;
}

float getDegreeOfWet() {
    return dhtSensor.readHumidity();
}

float getTemperature() {
    tempSensor.requestTemperatures();
    return (tempSensor.getTempCByIndex(0) + dhtSensor.readTemperature()) / 2;
}

String getAllInfo() {
    /*
  * will return temperature, degree of wet, UVs current
  */
    String time = getTime();
    String UVs = String(getUV());
    String wet = String(getDegreeOfWet());
    String temp = String(getTemperature());
    return time + ", " + wet + ", " + temp + ", " + UVs;
}

void printCurrentTime() {
    String currentTime = getTime();
    Serial.println(currentTime);
    Serial.print(morning);
    Serial.print("\t");
    Serial.print(night);
    Serial.print("\t");
    Serial.print(irrigateDuration);
    Serial.print("\n");
    delay(1000);
}

void doIrrigate() {
    if (had_been_irrigated == false) {
        had_been_irrigated = true;
        digitalWrite(RELAY_PIN, true);
        delay(irrigateDuration);
        digitalWrite(RELAY_PIN, false);
    }
}

void bluetooth_control() {
    String readin = "0";
    if (BT.available()) {
        readin = BT.readString();
        Serial.print(readin);
        Serial.write("\n");
        if (readin[0] == '0')
            return;
        else if (readin[0] == 'm')  //set morning doIrrigate time
            morning = (readin[1] - '0') * 10 + (readin[2] - '0');

        else if (readin[0] == 'a')  //set afternoon doIrrigate time
            night = (readin[1] - '0') * 10 + (readin[2] - '0');

        else if (readin[0] == 's') {  //set irrigating duration time
            int index = 1;
            unsigned long tmp = 0;
            while (readin[index] != 'q') {
                tmp = tmp * 10 + (readin[index] - '0');
                index++;
            }
            irrigateDuration = tmp;
        }

        else if (readin[0] == 'w')  // open the relay
            digitalWrite(RELAY_PIN, readin[1] - '0');
        else
            Serial.print("readin failed!\n");
    }
}

void setup() {
    Serial.begin(9600);
    BT.begin(9600);
    Wire.begin();  // Start the I2C interface
    tempSensor.begin();
    dhtSensor.begin();
    pinMode(RELAY_PIN, OUTPUT);

    //setTime();  //just upload it in the first time
    irrigateDuration = 370370;  //ms
    morning = 6;
    night = 16;
}

void loop() {
    printCurrentTime();
    int current_hour = Clock.getHour(false, setPM);

    if (current_hour == morning || current_hour == night)
        doIrrigate();
    else if (current_hour == morning + 1 || current_hour == night + 1)
        had_been_irrigated = false;

    bluetooth_control();
}
