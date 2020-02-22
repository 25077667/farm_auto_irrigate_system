#include <DHT.h> //depend on Adafruit_Sensor, didn't install yet
#include <DS3231.h>
//#include <DallasTemperature.h>
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
GUVA_S12SD uvSensor;                    // UVs
//OneWire oneWire(ONE_WIRE_PIN);          // wire of temp
//DallasTemperature tempSensor(&oneWire); // temperature
DHT dhtSensor(DHT_PIN, DHTTYPE);

struct Goal {
  float wet, temperature;
  int uvIndex;
  Goal() {}
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

unsigned int cal_checksum(unsigned int _temp, unsigned int _humi,
                          unsigned int _uv, unsigned int _piHerght,
                          bool _isAuto) {

  return ((((_temp % 127 + _humi % 127) % 127 + _uv % 127) % 127 +
           _piHerght % 127) % 127 + (int)_isAuto) % 127;
}

bool checkSum(unsigned int _temp, unsigned int _humi, unsigned int _uv,
              unsigned int _piHerght, bool _isAuto, unsigned int _check) {
  unsigned int check_res = cal_checksum(_temp, _humi, _uv, _piHerght, _isAuto);
  return check_res == _check;
}

unsigned long currentTimeBySecond() {
  unsigned long days =
      (long)Clock.getYear() * 365 + (long)Clock.getMonth(Century) * 30 + (long)Clock.getDate();
  unsigned long seconds = ((long)Clock.getHour(h12, setPM)) * 3600 +
                          (long)Clock.getMinute() * 60 + (long)Clock.getSecond();
  return days * 86400 + seconds;
}

/**
  * DS3231 to arduino
  * VCC to 5V
  * GND to GND
  * D   to SDA
  * C   to SCL
  * otherwise not work!
  */
void setTime() {
  Clock.setYear(20);
  Clock.setMonth(2);
  Clock.setDate(21);
  Clock.setDoW(4);
  Clock.setHour(14);
  Clock.setMinute(37);
  Clock.setSecond(0);
}

String getCurrentTime() {
  return String(Clock.getMonth(Century)) + String("-") +
         String(Clock.getDate()) + String("-") + String(Clock.getYear()) +
         String(" ") + String((int)Clock.getHour(h12, setPM)) + String(":") +
         String(Clock.getMinute()) + String(":") + String(Clock.getSecond());
}

int getUV() { return (int)uvSensor.UVIndex() + 1; }

float getDegreeOfWet() { return dhtSensor.readHumidity(); }

float getTemperature() {
  return dhtSensor.readTemperature();
}

String getAllInfo() {
  /*
   * will return temperature, degree of wet, UVs current
   */
  unsigned int _temp_i = int(getTemperature());
  unsigned int _humi_i = int(getDegreeOfWet());
  unsigned int _uv_i = int(getUV());
  
  String _time = getCurrentTime();
  String temp = String(_temp_i);
  String wet = String(_humi_i);
  String UVs = String(_uv_i);
  /**
   * cal_checksum()
   * @para
   * _temp = temp
   * _herhi = wet
   * _uv = USs
   * this machine ID = 0
   * isAutoNet set const 0, because this parameter it not works here
   */
  String check = String(cal_checksum(_temp_i, _humi_i, _uv_i, 0, 0));   
  return _time + ",0," + temp + "," + wet + "," + UVs + "," + check;
}

void printCurrentTime() {
  String currentTime = getCurrentTime();
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
    // relay net invert
    ;
}

void doCoolDown() {
  if (abs(getTemperature() - goal.temperature) >= 2) {
    // spray or do something
  }
}

void action() {
  unsigned long current = currentTimeBySecond();
  if (current - prevCheckTime >= actPeriod) {
    if (getDegreeOfWet() < goal.wet)
      doIrrigate();
    if (getTemperature() > goal.temperature)
      doCoolDown();
    if (autoMode && getUV() > goal.uvIndex)
      doNet(getUV() > goal.uvIndex);
    prevCheckTime = current;
  }
}



void serialInput() {
  if (Serial.available()) {
    String _temp = Serial.readStringUntil(',');
    String _humi = Serial.readStringUntil(',');
    String _uv = Serial.readStringUntil(',');
    String _piHeight = Serial.readStringUntil(',');
    String _isAutoNet = Serial.readStringUntil(',');
    String _check = Serial.readStringUntil('\n');

    if (_temp == "" || _humi == "" || _uv == "" || _piHeight == "" ||
        _isAutoNet == "" || _check == "")
      return; // worng Serial

    unsigned int _temp_i = _temp.toInt();
    unsigned int _humi_i = (int)_humi.toFloat();
    unsigned int _uv_i = _uv.toInt();
    unsigned int _piHeight_i = _piHeight.toInt();
    bool _isAutoNet_i = _isAutoNet.toInt();
    unsigned int _check_i = _check.toInt();
    
    if (checkSum(_temp_i, _humi_i, _uv_i, _piHeight_i, _isAutoNet_i, _check_i) == false)
      return; // worng checkSum

    goal.temperature = _temp_i;
    goal.wet = _humi_i * 0.01;
    goal.uvIndex = _uv_i;
    autoMode = _isAutoNet_i;
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
  Wire.begin(); // Start the I2C interface
  uvSensor.initialize(A0);
  dhtSensor.begin();
  pinMode(RELAY_PIN, OUTPUT);

  //setTime();  //just upload it in the first time
  irrigateDuration = 370370; // ms
  morning = 6;
  night = 16;
  actPeriod = 900; // 15min
  prevCheckTime = 0;
  goal = Goal(0.7, 21, 7);
  autoMode = true;
}

void loop() {
  //printCurrentTime();
  //Serial.println(getAllInfo());
  //Serial.println(getDegreeOfWet()); //get nan
  //Serial.println(getTemperature());
  //Serial.println(getCurrentTime());
  //delay(1000);
  
  serialInput();
  action();
  //delay(1000);
  serialOutput();
}
