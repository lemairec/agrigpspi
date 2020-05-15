#include <Arduino.h>

String a;


#define MOTOR_ENABLE_PIN 10 //pwm
#define MOTOR_PIN1 8
#define MOTOR_PIN2 7

#define MOTOR_ENCODER A1


void version(){
  Serial.println("PILOT_0_0_1");
}

void turnMotor2(int pas, boolean reverse){
  int i = 0;
  int oldValue = 0;

  SetMotor2(255, reverse);
  
  while(i<pas){
    int  read = digitalRead(MOTOR_ENCODER);
    if(read!=oldValue){
      if(read==1){
        ++i;
      }
      oldValue=read;
    }
  }
  SetMotor2(0, reverse);
  
}

void SetMotor2(int speed, boolean reverse)
{
  analogWrite(MOTOR_ENABLE_PIN, speed);
  digitalWrite(MOTOR_PIN1, ! reverse);
  digitalWrite(MOTOR_PIN2, reverse);
}
void setup(){
  pinMode(MOTOR_ENCODER, INPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  
  Serial.begin(9600);
  version();
 
}




void loop(){
  readNextFrame();
}

int m_bufferIndLast = 0;
char m_buffer[200];

void resetBuffer(){
  m_bufferIndLast = 0;
}


void readNextFrame(){
    while ( Serial.available()){
        char c = Serial.read();
        readChar(c);
    }
}

void readChar(char c){
  if(c == '$'){
    resetBuffer();
  } else if(c == '\n'){
    parseBuffer();
  } else {
    m_buffer[m_bufferIndLast] = c;
    m_bufferIndLast++;
  }
}

int myReadChar(char c){
  if(c == '1'){
    return 1;
  } else if(c == '2'){
    return 2;
  } else if(c == '3'){
    return 3;
  } else if(c == '4'){
    return 4;
  } else if(c == '5'){
    return 5;
  } else if(c == '6'){
    return 6;
  } else if(c == '7'){
    return 7;
  } else if(c == '8'){
    return 8;
  } else if(c == '9'){
    return 9;
  } else {
    return 0;
  }
}


void parseBuffer(){
  if(m_buffer[0] == 'H'){
    Serial.println("");
    Serial.println("$R;1");
    Serial.println("$L;100");
    Serial.println("$V");
    Serial.println("");
  } else if(m_buffer[0] == 'V'){
    version();
  } else if(m_buffer[0] == 'I' && m_buffer[1] == 'N' && m_buffer[2] == 'I' && m_buffer[3] == 'T'){
    int pin = myReadChar(m_buffer[5]);
    pinMode(pin, OUTPUT);
    Serial.print("$INIT;");
    Serial.println(pin);
  } else if(m_buffer[0] == 'R'){
    int res = 0;
    for(int i=2; i<m_bufferIndLast-1; ++i){
      res = res*10+myReadChar(m_buffer[i]);
    }
    goRight(res);
  } else if(m_buffer[0] == 'L'){
    int res = 0;
    for(int i=2; i<m_bufferIndLast-1; ++i){
      res = res*10+myReadChar(m_buffer[i]);
    }
    goLeft(res);
  }  else {
    Serial.println("$error");
  }
}

int my_delay = 200;

void goRight(int l){
  
  Serial.print("Right ");
  Serial.println(l);
  turnMotor2(l,true);
  Serial.println("right ok");
}

void goLeft(int l){
  Serial.print("Left ");
  Serial.println(l);
  turnMotor2(l,false);
  Serial.println("left ok");
}