#include <Arduino.h>

String a;

void version(){
  Serial.println("BINEUSE_0_0_1");
}

void setup(){
  Serial.begin(9600);
  version();

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(3, OUTPUT);
    Serial.println("Ok");


  
}

void loop(){
  for(int i =0;i<100;++i){
      digitalWrite(3, HIGH);
  delayMicroseconds(100);
    digitalWrite(3, LOW);
  delayMicroseconds(100);
  }
    Serial.println("Ok");

}
