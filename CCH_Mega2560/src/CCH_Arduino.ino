#include <Arduino.h>

unsigned int interval = 1000/30; //Milliseconds
//                                        13            19           24                                     38 40         45                                   58~
//const int pins[] = {3,4,5,6,7,8,9,10,11,12,14,15,16,17,18,20,21,22,23,25,26,27,28,29,30,31,32,33,34,35,36,37,39,41,42,43,44,46,47,48,49,50,51,52,53,54,55,56,57,A5,A6,A7,A8,A9,A10,A11,A12,A13};
const int pins[] = {68,39,18,62,20,34,11,21,22,5,9,8,14,17,25,7,32,12,4,10,3,31,65,15,29,33,16,27,67,23,56,48,30,42,50,49,55,41,37,26,44,43,51,54,47,6,46,36,52,57,28,35,53};
const int cameraCount = sizeof(pins) / sizeof(pins[0]);
const unsigned long long int msb = 2ULL << (cameraCount - 1);
//const unsigned long long int one = 1;
unsigned long long int data = 0;
unsigned long int now = 0;
bool btnready = true;

inline void trigger() {
  data += (int)!(data % 2);
}

void setup() {
  pinMode(2,INPUT_PULLUP);
  for(int i=0; i<cameraCount; ++i) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], 1);
  }
  //interrupt: button -> trigger()
 // attachInterrupt(2, trigger, FALLING);
  Serial.begin(115200);
  delay(1000);

  //int a = 68;pinMode(a,OUTPUT);digitalWrite(a,0);delay(100);digitalWrite(a,1);
  Serial.print("Welcome to Camera Controller App.\nUsage:\n  send number to set framerate and fire immediately.\n  1 = 1/10s, 2 = 1/20s, ... n = 1/n0s\n  send 0 to fire all devices.");
}

int input;

void loop() {
  if(Serial.available()) {
    input = Serial.read() - 48; // take only first byte
    // 3 -> 1/30 s , 5 -> 1/50 s between neighbours
    delay(10);
    //Serial.println(input);
    if(input == 0) {
      for(int i=0; i<cameraCount; ++i) {digitalWrite(pins[i], 0);}
      delay(200);
      for(int i=0; i<cameraCount; ++i) {digitalWrite(pins[i], 1);}
    } else if(input > 0 && input<68) {
      interval = 100 / input;
      btnready = false; trigger();
    } else if(input == 68) {
      //trigger single
      int tpin = Serial.parseInt();
      digitalWrite(tpin, 0);
      delay(100);
      digitalWrite(tpin, 1);
    }
    while(Serial.available()) {Serial.read();}
    input = 1000;
  }
  if(millis() - now >= interval) {
    now = millis();
    if(data >= msb) {data -= msb;}
    //Serial.print("data");
    //Serial.println(String(data,BIN));
    for(int i=0; i<cameraCount; ++i) {
      digitalWrite(pins[i], (!(data & (1ULL << i))));
    }
    data = data << 1;
    if(digitalRead(2) == LOW && btnready) {btnready = false; trigger();}
    else if(digitalRead(2) == HIGH) {btnready = true;}
  }
}
