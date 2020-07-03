#include <Arduino.h>

String a;

#define MOTOR_ON 255

#define MOTOR_ENABLE_PIN 10 //pwm
#define MOTOR_PIN1 8
#define MOTOR_PIN2 7

#define ENCODER_A 2
#define ENCODER_B 3

int position = 0;
int desired_position = 0;

int encoder_last_state_A;
int state_A;
int state_B;

void version(){
  Serial.println("#PILOT_0_0_3");
}

void updatePosition(){
  state_A = digitalRead(ENCODER_A); // Reads the "current" state of the outputA
  state_B = digitalRead(ENCODER_B); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (state_A != encoder_last_state_A){     
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (state_B != state_A) { 
      position ++;
    } else {
      position --;
    }
    Serial.print("$P;");
    Serial.println(position);
  } 
  encoder_last_state_A = state_A; // Updates the previous state of the outputA with the current state
}

void goTo(int res){
  desired_position = res;
  if(position < desired_position){
    turnLeftMotorGoTo();
  } else {
    turnRightMotorGoTo();
  }
}



void turnRightMotorGoTo(){
  /*Serial.print("turnRightMotor");
  Serial.print(desired_value);
  Serial.println("");*/
  SetMotor2(MOTOR_ON, 0);
  //int oldValue = 0;
  while(position < desired_position && !Serial.available()){
    updatePosition();
  }
  SetMotor2(0,0);
}

void turnLeftMotorGoTo(){
  /*Serial.print("turnLeftMotor ");
  Serial.print(desired_value);
  Serial.println("");*/
  SetMotor2(MOTOR_ON, 1);
  //int oldValue = 0;
  while(position > desired_position && !Serial.available()){
    updatePosition();
  }
  SetMotor2(0,1);
}

/*void turnMotor2(int pas, boolean reverse){
  int i = 0;
  int oldValue = 0;

  SetMotor2(MOTOR_ON, reverse);
  
  while(i<pas && !Serial.available()){
    int  read = digitalRead(MOTOR_ENCODER);
    if(read!=oldValue){
      if(read==1){
        ++i;
      }
      oldValue=read;
    }
  }
  SetMotor2(0, reverse);
  
}*/



void SetMotor2(int speed, boolean reverse)
{
  analogWrite(MOTOR_ENABLE_PIN, speed);
  digitalWrite(MOTOR_PIN1, ! reverse);
  digitalWrite(MOTOR_PIN2, reverse);
}

/**
 * PARSEUR
 **/

int m_bufferIndLast = 0;
char m_buffer[200];

void resetBuffer(){
m_bufferIndLast = 0;
}


void readNextFrame(){
    if ( Serial.available()){
        char c = Serial.read();
        readChar(c);
    }
}

void readChar(char c){
  if(c == '$'){
    resetBuffer();
  } else if(c == '\n'){
    parseBuffer();
    resetBuffer();
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

void printBuffer(){
  for(int i=0; i<m_bufferIndLast-1; ++i){
    Serial.print(m_buffer[i]);
  }
  Serial.println("");
}

void parseBuffer(){
  printBuffer();
  if(m_buffer[0] == 'H'){
    Serial.println("");
    Serial.println("$R;100");
    Serial.println("$L;100");
    Serial.println("$G;100");
    Serial.println("$C");
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
  } else if(m_buffer[0] == 'C'){
    desired_position = 0;
    position = 0;
  } else if(m_buffer[0] == 'G'){
    int res = 0;
    for(int i=3; i<m_bufferIndLast-1; ++i){
      /*int j = myReadChar(m_buffer[i]);
      Serial.print(i);
      Serial.print(" ");
      Serial.print(m_buffer[i]);
      Serial.print("=>");
      Serial.print(j);
      Serial.println("");*/
      res = res*10+myReadChar(m_buffer[i]);
    }
    Serial.println(res);
    if(m_buffer[2] == '-'){
      res = -res;
    }
    goTo(res);
  }  else {
    Serial.println("$error");
  }
}

int my_delay = 200;

void goRight(int l){
  
  //Serial.print("Right ");
  //Serial.println(l);
  //turnMotor2(l,true);
  //Serial.println("right ok");
}

void goLeft(int l){
 // Serial.print("Left ");
  //Serial.println(l);
  //turnMotor2(l,false);
  //Serial.println("left ok");
}

void setup(){
  pinMode (ENCODER_A,INPUT);
  pinMode (ENCODER_B,INPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  
  encoder_last_state_A = digitalRead(ENCODER_A); 
  Serial.begin(115200);
  version();
 
}




void loop(){
  readNextFrame();
  updatePosition();
}
