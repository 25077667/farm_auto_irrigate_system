#include <SoftwareSerial.h>
#include <DS1302.h>

// 時間寫不上去的話，拔電池等5秒，重刷主機板再裝電池
// DS1302真的很糟糕，有時候時間會往前跳，有時候不會
// 最後我的修正方式就是delay一秒再判斷處理
// 希望永無bug!!!

#define RTC_RST 10
#define RTC_DAT 9
#define RTC_CLOCK 8
#define relay_pin 2
// relay_pin是給繼電器用的

DS1302 rtc(10, 9, 8);
SoftwareSerial BT(7, 6); //arduino RX/TX

String sth = ""; //接受時間回傳值
String hh = "";
String mm = "";
String ss = "";

bool water_flag = true;  //調控澆水開關，true代表要澆水
bool time_revise_flag = true;  //true 要改時間
int count = 1;


void set_time(){
  rtc.writeProtect(false);
  rtc.setDOW(SATURDAY);        // 設定週幾，如FRIDAY
  rtc.setTime(17, 0, 0);     // 設定時間 時，分，秒 (24hr format)
  rtc.setDate(7, 21, 2018);   // 設定日期 日，月，年
}
void show_time(){
  for(int i=0; i<2;i++){
    Serial.println(rtc.getDateStr());
    Serial.println(rtc.getTimeStr());
    delay(1000);
  }
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

void time_revising(String hh, String mm, String ss){
  /*
    if(time_revise_flag == true && hh == "00" && mm=="18" ){
    rtc.setTime(0,3,0);
    time_revise_flag = false;
  }
  */
  int hh_int = hh.toInt(), mm_int = mm.toInt(), ss_int = ss.toInt();

  delay(1000);
  String get_time_after_a_second = rtc.getTimeStr();
  String dm = get_time_after_a_second.substring(3,5);
  String ds = get_time_after_a_second.substring(6,8);

  if( ((ds.toInt()-ss_int) != 1) && ss_int <= 59){
    rtc.setTime(hh_int, mm_int , ss.toInt()+1);
  }
  if(ds.toInt() > 59){
    rtc.setTime(hh_int, mm_int+1 , 0);
  }

}

void setup() {
  rtc.halt(false);
  Serial.begin(9600);
  BT.begin(9600);
  //set_time();  //第一次傳就好
  pinMode(relay_pin,OUTPUT);
}

void loop() {
  //show_time();
  time_revise_flag = true;
  String readin = "0";
  sth = rtc.getTimeStr();
  hh = sth.substring(0,2);
  mm = sth.substring(3,5);
  ss = sth.substring(6,8);

  int hhh = hh.toInt();
  if ( hhh == 5 || hhh == 19)
    water(); //註解1號
  if ( hhh == 6 || hhh == 20){
    time_revise_flag = true;
    water_flag = true; //註解二號
  }

  // 早上5點會去澆水(呼叫water()函式)，然後water會把water_flag關掉，早上6點再把water_flag打開，直到19點符合註解1號
  // 因為只要是早上5點，就不會是早上6點，所以關了之後6點再開就不會撞到5點
  // 早上6點，把time_revise_flag打開，讓晚上00:18可以再用

  if(BT.available()){
    readin = BT.readString();
    Serial.print(readin);
    Serial.write("\n");
    //==========================================================
    if(readin[0]=='s')
    {
      time_revise_flag = false;
      String string_h = readin.substring(1,3), string_m = readin.substring(4,6), string_s = readin.substring(7,9);
      int get_h = string_h.toInt(); //季的去app inventor增加輸出判斷，萬一分鐘=0，或是秒鐘=0，要把輸出強制改成瞭位數
      int get_m = string_m.toInt();
      int get_s = string_s.toInt();
      Serial.print(get_h);Serial.print("\n");Serial.print(get_m);Serial.print("\n");Serial.print(get_s);Serial.print("\n");
      rtc.setTime(get_h, get_m , get_s);
      
      //===================================================================================

    }
    else
      digitalWrite(relay_pin, readin[0]-'0');
  }

  if(hhh != 5 && hhh != 19 && time_revise_flag == true)
    time_revising(hh, mm, ss);
}
