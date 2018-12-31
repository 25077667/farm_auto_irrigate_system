#include <DS3231.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define watering_delay 185185
#define morning 6
#define night 16
#define relay_pin 2

DS3231 Clock;
bool h12;
bool PM;
bool Century=false;
bool had_been_irrigated = false;
SoftwareSerial BT(7, 6); //arduino RX/TX

void set_time(){
  Clock.setYear(18);
  Clock.setMonth(12);
  Clock.setDate(9);
  Clock.setDoW(0);
  Clock.setHour(11);	//the format is 24 hours
  Clock.setMinute(44);
  Clock.setSecond(0);
}

void show_time(){
      Serial.print(Clock.getYear(), DEC);
      Serial.print("-");
      Serial.print(Clock.getMonth(Century), DEC);
      Serial.print("-");
      Serial.print(Clock.getDate(), DEC);
      Serial.print(" ");
      Serial.print((int)Clock.getHour(h12,PM), DEC); //24-hr
      Serial.print(":");
      Serial.print(Clock.getMinute(), DEC);
      Serial.print(":");
      Serial.println(Clock.getSecond(), DEC);
      delay(1000);
}

void water(){
  if ( had_been_irrigated == false ){
    had_been_irrigated = true;
    digitalWrite(relay_pin, true);
    delay(watering_delay);
    digitalWrite(relay_pin, false);
  }
}

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  Wire.begin();// Start the I2C interface
  pinMode(relay_pin,OUTPUT);
  /*set_time();  just upload it in the first time*/
}

void loop(){
  //show_time();
  String readin = "0";
  int current_hour = Clock.getHour(h12,PM);

  if(current_hour == morning || current_hour == night)
	  water();
  else if(current_hour == morning+1 || current_hour == night+1)
	  had_been_irrigated = false;

  if(BT.available()){
    readin = BT.readString();
    Serial.print(readin);
    Serial.write("\n");
    digitalWrite(relay_pin, readin[0]-'0');
    //if readin is 1, that is open the water gate
  }

}
