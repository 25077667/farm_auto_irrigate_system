#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;
bool h12;
bool PM;
bool Century=false;


void setup() {
	// Start the serial port
	Serial.begin(9600);

	// Start the I2C interface
	Wire.begin();
  Clock.setYear(18);
  Clock.setMonth(10);
  Clock.setDate(6);
  Clock.setDoW(6);
  Clock.setHour(17);
  Clock.setMinute(13);
  Clock.setSecond(0);

}

void loop() {
    for (int i=0; i<5; i++){
      delay(1000);
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
    }
}

