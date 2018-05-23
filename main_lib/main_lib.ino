#include <SoftwareSerial.h>
#include <DS1302.h>


#define RTC_RST 10
#define RTC_DAT 9
#define RTC_CLOCK 8
#define BT_RX 7
#define BT_TX 6


String sth = ""; //接受時間回傳值
String hh = "";
String mm = "";
String ss = "";

String message="";

#define relay_pin 2

DS1302 rtc(10, 9, 8);
SoftwareSerial BT(7, 6);

void set_time(){
  rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setDOW(FRIDAY);        // 設定週幾，如FRIDAY
  rtc.setTime(11, 47, 0);     // 設定時間 時，分，秒 (24hr format)
  rtc.setDate(5, 23, 2016);   // 設定日期 日，月，年
}
void show_time(){
  for(int i=0; i<10;i++){
    Serial.println(rtc.getTimeStr());
    delay(1000);
  }
  
}
void water(){
  // average output water in balence line is 69.6 ml/s
  //evaluate 8 plants in our project
  digitalWrite(relay_pin, 1);
  delay(20000);   //therefore we pull over 1392ml/time
  digitalWrite(relay_pin, 0);
}

void auto_pull(int mode){
  // this is debug mode
  // continue pull out water untill you stop
  if ( mode == 1 )
    digitalWrite(relay_pin, 1);
  else
    digitalWrite(relay_pin, 0);
  return true;
}
void check_bt_input(){
  while(Serial.available()){
    message = char(Serial.read());
    if ( message == '1'){
      auto_pull(1);
      break;
    }
    if ( message == '0'){
      auto_pull(0);
      break;
    }
  }
}

void setup() {
  BT.begin(9600);
  Serial.begin(9600);
  //set_time();  //第一次傳就好
  pinMode(relay_pin,OUTPUT);
}

void loop() {
  //Serial.println("hello");
  //delay(600000);
  //show_time();
  sth = rtc.getTimeStr();
  hh = sth.substring(0,2);
  //mm = sth.substring(3,5);
  //ss = sth.substring(6,8);
  int hhh = hh.toInt();
  if (hhh == 6 || hhh == 19)
    water();
    
  check_bt_input();
  delay(10000);
}
