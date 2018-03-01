/*
 * Code to control RC bot's servos and LED Lives counter
 * By: Aryton Hoi
 */
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//Setup NRF Wireless Communication Module
RF24 radio(8,9); // CE, CSN
//Setup radio address
//Change address to "00002" for second robot
const byte address[6] = "00001";

//Create servo objects
Servo servo1;   //right servo
Servo servo2;   //left servo

//Setup life-counter variables and values
const int deathTimer = 2000;
int timeOfDeath = 0;
int currentTime = 0;
const int target = A0;
int targetState;
int lives = 3;

//Setup lives LED indicator pins
const int led1 = 7;
const int led2 = 4;
const int led3 = 2;

void setup()                                 
{ 
  //Serial communication for debuging
  //Serial.begin(9600);
  
  //Begin wireless communication
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  //Set NRF as receiver and start listening for transmission data
  radio.startListening();

  //Attach servos to pins
  servo1.attach(6);
  servo2.attach(5);

  //Setup target button pin
  pinMode(target, INPUT);
  //Setup lives LED pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  //Turn lives LEDs on (3 lives)
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
}  
 
void loop(){
  while(lives > 0){
    if(timeOfDeath==0||(timeOfDeath + 2000 <= millis())){
      targetState = digitalRead(target);
      if(targetState == 1){
        lives--;
        timeOfDeath = millis();
      }
    }
    if(lives == 2){
      digitalWrite(led1, LOW);
    }
    if(lives == 1){
      digitalWrite(led2, LOW);
    }
    if(lives == 0){
      digitalWrite(led3, LOW);
    }
    if(radio.available())
    {
      //Receive transmission
      int transmit[4];            
      radio.read(&transmit, sizeof(transmit));
      Serial.print(transmit[2]);
      Serial.print(", ");
      Serial.println(transmit[3]);

      //right servo control
      if(transmit[2]==0){
        servo1.write(100);
      }else{
        servo1.write(190);
      }
      //left servo control
      if(transmit[3]==0){
        servo2.write(100);
      }else{
        servo2.write(20);
      }    
      delay(30);
    }
  }
  delay(10000);
  lives = 3;
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
}
  
