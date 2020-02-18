int isAllCorrect(){
  //return last wrong index
  int index = -1;
  int counter = 0;
  while(Serial.available()){
    char c = Serial.read();
    if(c != 'a')
      index = counter;
    counter++;
  }
  return index;
}

void setup(){
  Serial.begin(9600);
}
void loop(){
  if (Serial.available()){
    String msg = Serial.readStringUntil('\n');
    Serial.println(msg);
    delay(50);
  }
}
