#include <Arduino.h>

unsigned int interval = 1000/30; //Milliseconds
const int pins[] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};
const int cameraCount = sizeof(pins) / sizeof(pins[0]);
const unsigned long msb = 2 << (cameraCount - 1);
unsigned long data = 0;
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
  pinMode(2,INPUT_PULLUP);
  //interrupt: button -> trigger()
 // attachInterrupt(2, trigger, FALLING);
  Serial.begin(115200);
  delay(1000);
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
      digitalWrite(pins[i], (int)(!(data & (1 << i))));
    }
    data = data << 1;
    if(digitalRead(2) == LOW && btnready) {btnready = false; trigger();}
    else if(digitalRead(2) == HIGH) {btnready = true;}
  }
}
