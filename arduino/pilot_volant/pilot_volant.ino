#include <Arduino.h>

String a;

void version(){
  Serial.println("PILOT_0_0_1");
}


void setup(){
  Serial.begin(9600);
  version();

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  Serial.println("Ok");


  
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
    Serial.println("$R;010");
    Serial.println("$L;010");
    Serial.println("$D;010");
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
    int i0 = myReadChar(m_buffer[3]);
    int i1 = myReadChar(m_buffer[4]);
    int i2 = myReadChar(m_buffer[5]);
    int res = i0*100+i1*10+i2;
    goRight(res);
  } else if(m_buffer[0] == 'L'){
    int i0 = myReadChar(m_buffer[3]);
    int i1 = myReadChar(m_buffer[4]);
    int i2 = myReadChar(m_buffer[5]);
    int res = i0*100+i1*10+i2;
    goLeft(res);
  }  else {
    Serial.println("$error");
  }
}

int my_delay = 100;

void goRight(int l){
  Serial.print("Right");
  Serial.println(l);
  digitalWrite(6, HIGH);
  for(int i =0;i<l;++i){
    digitalWrite(3, HIGH);
    delayMicroseconds(my_delay);
    digitalWrite(3, LOW);
    delayMicroseconds(my_delay);
  }
}

void goLeft(int l){
  Serial.print("Left");
  Serial.println(l);
  digitalWrite(6, LOW);
  for(int i =0;i<l;++i){
    digitalWrite(3, HIGH);
    delayMicroseconds(my_delay);
    digitalWrite(3, LOW);
    delayMicroseconds(my_delay);
  }
}