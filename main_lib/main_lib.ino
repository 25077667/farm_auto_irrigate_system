#include <DS1302.h>

#define RTC_RST 10
#define RTC_DAT 9
#define RTC_CLOCK 8

String sth = ""; //接受時間回傳值
String hh = "";
String mm = "";
String ss = "";

#define relay_pin 2

DS1302 rtc(10, 9, 8);

void set_time(){
  rtc.setDOW(FRIDAY);        // 設定週幾，如FRIDAY
  rtc.setTime(21, 35, 0);     // 設定時間 時，分，秒 (24hr format)
  rtc.setDate(5, 8, 2016);   // 設定日期 日，月，年
}

void water(){
  
}

void setup() {
  Serial.begin(9600);
  //set_time();  //第一次傳就好
  
}

void loop() {
  //Serial.println("hello");
  //delay(1000);
  sth = rtc.getTimeStr();
  hh = sth.substring(0,2);
  //mm = sth.substring(3,5);
  //ss = sth.substring(6,8);
  int hhh = hh.toInt();
  if (hhh == 6 || hhh == 19)
    water();
}
