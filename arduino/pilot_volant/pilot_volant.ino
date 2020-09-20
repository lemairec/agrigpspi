#include <Arduino.h>

String a;


#define MOTOR_ENABLE_PIN 3 //pwm
#define MOTOR_PIN1 4
#define MOTOR_PIN2 2

#define ENCODER_A 10
#define ENCODER_B 11

long position = 0;
long desired_position = 0;

int encoder_last_state_A;
int state_A;
int state_B;

int motor_max = 150;

void version(){
    Serial.println("$PILOT_0_0_3");
}

void printPosition(){
    Serial.print("$P;");
    Serial.println(position);
}

int i = 0;
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
    ++i;
    if(i>5){
      i = 0;
      printPosition();
    }

    
  } 
  //Serial.print("$P;");Serial.print(state_A);Serial.print(" ");Serial.println(state_B);
  encoder_last_state_A = state_A; // Updates the previous state of the outputA with the current state
}

void goTo(long res){
  desired_position = res;
  //Serial.print("Go to ");Serial.print(desired_position);Serial.print(" ");Serial.println(position);
  updateMotor();
  
}

void updateMotor(){
  int i = 0;
  while(!Serial.available()){
    long abs = 5*(position - desired_position);
    if(i%100 == 0){
      //Serial.print(abs);Serial.print(" p ");Serial.print(position);Serial.print(" - ");Serial.print(desired_position);Serial.print(" = ");Serial.println((position - desired_position));
    }
    if(abs>250){
      abs=255;
    }
    if(abs<-250){
      abs=-255;
    }
    //erial.print(abs);
    if(abs <0){
      SetMotor2(-abs, 0);
    } else {
      SetMotor2(+abs, 1);
    }
    //Serial.println(abs);
    updatePosition(); ++i;
  }
  Serial.println("ok");
  SetMotor2(0,0);
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
  //Serial.print("SetMotor2 speed ");Serial.println(speed);
  if(speed > 255){
    analogWrite(MOTOR_ENABLE_PIN, 255);
      
  } else {
    analogWrite(MOTOR_ENABLE_PIN, speed);
  }
  digitalWrite(MOTOR_PIN1, ! reverse);
  digitalWrite(MOTOR_PIN2, reverse);
}

/**
 * PARSEUR
 **/

int m_bufferIndLast = 0;
char m_buffer[200];
int m_tempInd = 0;

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

long myReadInt(){
    long res = 0;
    bool inv = false;
    if(m_buffer[m_tempInd] == '-'){
        inv = true;
        m_tempInd++;
    }
    while(m_tempInd < m_bufferIndLast){
        char c = m_buffer[m_tempInd];
        if(c == ',' || c == ';'){
            ++m_tempInd;
            break;
        } else {
            res = res*10 + myReadChar(c);
            ++m_tempInd;
        }
        updatePosition();
    }
        
    if(inv){
        return -res;
    } else {
        return res;
    }
}

void printBuffer(){
    Serial.print("##");
    for(int i=0; i<m_bufferIndLast-1; ++i){
        Serial.print(m_buffer[i]);
    }
    Serial.println("");
}

void parseBuffer(){
  //printBuffer();
  updatePosition();
  if(m_buffer[0] == 'H'){
    Serial.println("");
    Serial.println("$G;100  //goto 100");
    Serial.println("$D      //disable");
    Serial.println("$V      //give version");
    Serial.println("$P      //print position");
    Serial.println("$C;100  //config; motor max");
    Serial.println("$L;255  //left; motor 255");
    Serial.println("$R;255  //right; motor 255");
    Serial.println("$S;     //stop");
    
    Serial.println("");
  } else if(m_buffer[0] == 'V'){
    version();
  } else if(m_buffer[0] == 'R'){
    int speed = myReadInt();
    m_tempInd = 2;
    //Serial.print("#R;");Serial.println(speed);
    SetMotor2(speed, false);
  } else if(m_buffer[0] == 'L'){
    int speed = myReadInt();
    m_tempInd = 2;
    //Serial.print("#L;");Serial.println(speed);
    SetMotor2(speed, true);
  } else if(m_buffer[0] == 'S'){
    SetMotor2(0, true);
  } else if(m_buffer[0] == 'C'){
    m_tempInd = 2;
    motor_max = myReadInt();
    Serial.print("#C;");Serial.println(motor_max);
  } else if(m_buffer[0] == 'D'){
    
  } else if(m_buffer[0] == 'G'){
    m_tempInd = 2;
    long res = myReadInt();
    Serial.print("#G;");Serial.println(res);
    goTo(res);
  } else if(m_buffer[0] == 'P'){
    printPosition();
  }  else {
    printBuffer();
    Serial.println("$error");
    
  }
}

int my_delay = 200;

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
