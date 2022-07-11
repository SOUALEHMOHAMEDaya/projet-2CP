#include <Servo.h>
#include <SoftwareSerial.h> 
  boolean lBlocked,rBlocked;
  //motors  pins
  const int led1 = 0;
  const int led2 = 1;
  const int ln1 = 4;
  const int ln2 = 5;
  const int ln3 = 6;
  const int ln4 = 7;
  
  //servo motor class
  Servo servo;
  
  // ultra Sensor PINS
  const int trig = 2;
  const int echo = 3;
  
  // calculation variables
  long lecture_echo;
  long cm;

//bluetooth variable
SoftwareSerial MyBlue(10, 11); // bluetooth TX  | bluetooth RX

//command variable for manual mode
char command;

//boolean trigger manual mode variable
boolean manual = false;

void setup(){
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
  servo.attach(8);
  servo.write(90);
  //motors pins
  pinMode(ln1,OUTPUT);
  pinMode(ln2,OUTPUT);
  pinMode(ln3,OUTPUT);
  pinMode(ln4,OUTPUT);
  MyBlue.begin(9600);
}

void loop(){
//manual mode code
//----------------------------------------------------------------------------------------------------------------
if (MyBlue.available()>0) {
  servo.write(90);
  manual = true;
  command = MyBlue.read();
  switch(command){
    case'L': {rotationLeft();
                break;}
        case'R': {rotationRight();
                break;}
        case'B': {backward();
                delay(500);
                Stop();
                break;}
        case'F': {forward();
                delay(500);
                Stop();
                break;}
        case'S': {Stop();
                break;}
        case'V': {manual = false;
                break;}
  }
  while(manual){
    if (MyBlue.available()>0) {
      command = MyBlue.read();
      switch(command){
        case'L': {rotationLeft();
                break;}
        case'R': {rotationRight();
                break;}
        case'B': {backward();
                delay(500);
                Stop();
                break;}
        case'F': {forward();
                delay(500);
                Stop();
                break;}
        case'S': {Stop();
                break;}
        case'V': {manual = false;
                break;}
              }
          }
      }
  }


//----------------------------------------------------------------------------------------------------------------
  // automatique pilote part
      rBlocked = true;
      lBlocked = true;
      servo.write(90);

    if(distance()>20){
      // Move the car forward
      forward();
    }else {
        // Stop the car , forward is blocked
        Stop();
        //turn detector to the left 
        servo.write(0);
        delay(500);
    if(distance()>20){
        // Turn the car to the left
        rotationLeft();
        servo.write(90);
    }else {
        // Stop the car , Left is blocked
        Stop();
        //turn detector to the right
        servo.write(180);
        delay(1000);       
        if(distance()>20){
          // Turn the car to the right
          rotationRight();         
          servo.write(90);          
        } else {
            // go backward
            backward();
            delay(1000);
            Stop();          
            while(rBlocked && lBlocked){
              //turn detector to the left 
               servo.write(0);
               delay(1000);
               lBlocked = distance()<=20;
               if(distance()>20){
                  // Turn the car to the left
                  rotationLeft();             
                  servo.write(90);
               }else {                 
                  // Stop the car , Left is blocked
                  Stop();
                  //turn detector to the right
                  servo.write(180);
                  delay(1000);
                  rBlocked = distance()<=20;
                if(distance()>20){
                  // Turn the car to the right
                  rotationRight();       
                  servo.write(90);
                    }else{
                        backward();
                        delay(1000);
                        Stop();
                    }
                }
            }   
        }          
    }      
  }
}
// methods     
float distance(){
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    lecture_echo = pulseIn(echo,HIGH);
    cm = lecture_echo /58;
    return cm;
}

void forward(){
    digitalWrite(ln1,HIGH);
    digitalWrite(ln2,LOW);
    digitalWrite(ln4,HIGH);
    digitalWrite(ln3,LOW);
}

void Stop(){
    digitalWrite(ln1,LOW);
    digitalWrite(ln2,LOW);
    digitalWrite(ln3,LOW);
    digitalWrite(ln4,LOW);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
}

void backward(){
    digitalWrite(ln2,HIGH);
    digitalWrite(ln1,LOW);
    digitalWrite(ln3,HIGH);
    digitalWrite(ln4,LOW);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
}

void right(){
    digitalWrite(ln1,HIGH);
    digitalWrite(ln2,LOW);
    digitalWrite(ln4,LOW);
    digitalWrite(ln3,LOW);
}

void left(){
    digitalWrite(ln1,LOW);
    digitalWrite(ln2,LOW);
    digitalWrite(ln3,LOW);
    digitalWrite(ln4,HIGH);
}

void rotationLeft(){
    digitalWrite(ln1,LOW);
    digitalWrite(ln2,HIGH);
    digitalWrite(ln3,LOW);
    digitalWrite(ln4,HIGH);
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    delay(300);
    Stop();
}
void rotationRight(){
    digitalWrite(ln1,HIGH);
    digitalWrite(ln2,LOW);
    digitalWrite(ln4,LOW);
    digitalWrite(ln3,HIGH);
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    delay(300);
    Stop();
  }
