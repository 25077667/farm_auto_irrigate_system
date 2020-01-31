#include <DHT.h>  //depend on Adafruit_Sensor, didn't install yet
#include <DS3231.h>
#include <DallasTemperature.h>
#include <GUVA_S12SD.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#define RELAY_PIN 2
#define ONE_WIRE_PIN 3
#define DHT_PIN 4
#define DHTTYPE DHT11

DS3231 Clock;
bool setPM = false;
bool h12 = false;
bool Century = false;
bool autoMode = false;
int morning;
int night;
unsigned long irrigateDuration;
unsigned long actPeriod;
unsigned long prevCheckTime;
GUVA_S12SD uvSensor;                  //UVs
OneWire oneWire(ONE_WIRE_PIN);           //wire of temp
DallasTemperature tempSensor(&oneWire);  //temperature
DHT dhtSensor(DHT_PIN, DHTTYPE);

struct Goal {
    float wet, temperature;
    int uvIndex;
    Goal(){}
    Goal(float _wet, float _temp, int _uv) {
        wet = _wet;
        temperature = _temp;
        uvIndex = _uv;
    }
} goal;

int stringToInt(String _str) {
    // with q be the end of string
    // i'm not sure it exist '\0' in the end of string
    int index = 0;
    int tmp = 0;
    while (_str[index] != 'q') {
        tmp = tmp * 10 + (_str[index] - '0');
        index++;
    }
    return tmp;
}

unsigned long currentTimeBySecond() {
    unsigned long days = Clock.getYear() * 365 + Clock.getMonth(Century) * 30 + Clock.getDate();
    unsigned long seconds = (int)(Clock.getHour(h12, setPM))* 3600 + Clock.getMinute() * 60 + Clock.getSecond();
    return days * 86400 + seconds;
}

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
           String((int)Clock.getHour(h12, setPM)) + String(":") +
           String(Clock.getMinute()) + String(":") +
           String(Clock.getSecond());
}

int getUV() {
    return (int)uvSensor.UVIndex() + 1;
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
    digitalWrite(RELAY_PIN, true);
    delay(irrigateDuration);
    digitalWrite(RELAY_PIN, false);
}

void doNet(bool openNet) {
    if (openNet)
        // relay net on
        ;
    else
        //relay net invert
        ;
}

void doCoolDown() {
    if (abs(getTemperature() - goal.temperature) >= 2) {
        //spray or do something
    }
}

void action() {
    unsigned long current = currentTimeBySecond();
    if (current - prevCheckTime >= actPeriod) {
        if (getDegreeOfWet() < goal.wet)
            doIrrigate();
        if (getTemperature() > goal.temperature)
            doCoolDown();
        if (getUV() > goal.uvIndex)
            doNet(getUV() > goal.uvIndex);
        prevCheckTime = current;
    }
}

String serial2String(){
    String s = "";
    while (Serial.available()) {
        char c = Serial.read();
        if(c!='\n'){
            s += c;
        }
        else
          return s;
        delay(5);    // 沒有延遲的話 UART 串口速度會跟不上Arduino的速度，會導致資料不完整
    }
}

void serialInput() {
    if (Serial.available()) {
        String readIn = serial2String();
        
        if (readIn == String("NET"))
            doNet(true);
        else if (readIn == String("!NET"))
            doNet(false);
        else if (readIn.substring(0, 9) == "Cool down") {
            goal.temperature = stringToInt(readIn.substring(9));
            doCoolDown();
        } else if (readIn.substring(0, 9) == "Wet") {  // format: Wet90q, means 90% of wet in mod
            goal.wet = stringToInt(readIn.substring(3)) * 0.01;
            doIrrigate();
        } else if (readIn == "Auto mode")
            autoMode = true;
        else if (readIn == "!Auto mode")
            autoMode = false;
    }
}

void serialOutput() {
    if (Serial.available() && currentTimeBySecond() - prevCheckTime > actPeriod) {
        Serial.println(getAllInfo());
        prevCheckTime = currentTimeBySecond();
    }
}

void setup() {
    Serial.begin(9600);
    Wire.begin();  // Start the I2C interface
    uvSensor.initialize(A0);
    tempSensor.begin();
    dhtSensor.begin();
    pinMode(RELAY_PIN, OUTPUT);

    //setTime();  //just upload it in the first time
    irrigateDuration = 370370;  //ms
    morning = 6;
    night = 16;
    actPeriod = 900;  //15min
    prevCheckTime = 0;
    goal = Goal(0.7, 21, 7);
    autoMode = true;
}

void loop() {
    //printCurrentTime();
    serialInput();
    if (autoMode)
        action();
    serialOutput();
}
