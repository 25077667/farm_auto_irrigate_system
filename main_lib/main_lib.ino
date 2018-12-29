#include <DS3231.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// 希望永無bug!!!

#define relay_pin 2   // relay_pin是給繼電器用的
DS3231 Clock;
bool h12;
bool PM;
bool Century=false;
bool water_flag = true;  //調控澆水開關，true代表要澆水
SoftwareSerial BT(7, 6); //arduino RX/TX

void set_time(){
  Clock.setYear(18);
  Clock.setMonth(12);
  Clock.setDate(9);
  Clock.setDoW(0);
  Clock.setHour(11);	//24小時制
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
      int hh = Clock.getHour(h12,PM);
      Serial.print(hh, DEC); //24-hr
      Serial.print(":");
      Serial.print(Clock.getMinute(), DEC);
      Serial.print(":");
      Serial.println(Clock.getSecond(), DEC);
      delay(1000);
}

void water(){
  // 每秒 18 ml
  if ( water_flag == true ){
    water_flag = false;
    digitalWrite(relay_pin, 1);//繼電器啟動電磁閥
    delay(185185); //185秒多
    digitalWrite(relay_pin, 0);
  }
}

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  
  Wire.begin();// Start the I2C interface
  /*set_time();  第一次傳就好*/
  pinMode(relay_pin,OUTPUT);
}

void loop(){
  //show_time();
  String readin = "0";
  int hh = Clock.getHour(h12,PM);
  
  if(hh == 6 || hh == 16)
	  water();
	//早上5點 晚上7點澆水
  if(hh == 7 || hh == 17)
	  water_flag = true;

  if(BT.available()){
    readin = BT.readString();
    Serial.print(readin);
    Serial.write("\n");
    digitalWrite(relay_pin, readin[0]-'0');
    //強制開水
  }  

}
