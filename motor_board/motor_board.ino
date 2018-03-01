/*
 * Code to control RC bot motors
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

//Setup pisn to connect to L298N Motor Driver Module
// Right Motor A
const int enA = 6;
const int in1 = 7;
const int in2 = 5;
// Left Motor B
const int enB = 3;
const int in3 = 4;
const int in4 = 2;

//Setup Motor speed values - Start at zero
int MotorSpeed1 = 0;
int MotorSpeed2 = 0;

void setup()                                 
{ 
  //Serial communication for debuging
  //Serial.begin(9600);
  
  //Begin wireless communication
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  //Set NRF as receiver
  radio.startListening();

  // Set all the motor control pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Start with motors disabled and direction forward
  // Right Motor A
  digitalWrite(enA, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Right Motor B
  digitalWrite(enB, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}  
 
void loop(){
  if(radio.available())
  {
      //Setup array to recieve transmission data
      int transmit[4];            
      radio.read(&transmit, sizeof(transmit));

      //Serial print transmission values for debugging
      /*
      Serial.print(transmit[0]);
      Serial.print(", ");
      Serial.print(transmit[1]);
      Serial.print(", ");
      Serial.println(transmit[2]);
      */
      
      //Read transmission data to control Right Motor 1
      //If joystick goes backwards, setup L298 Module to drive motors backwards
      if(transmit[0] < 450){             
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        transmit[0] = transmit[0] - 450; // This produces a negative number
        transmit[0] = transmit[0] * -1;  // Make the number positive
        //Map joystick values to range between 0 to 255 to control DC motors
        MotorSpeed1 = map(transmit[0], 0, 450, 0, 255);
        //Send data to driver motor
        analogWrite(enA, MotorSpeed1);
      //If joystick goes forwards, setup L298 Module to drive motors forwards
      }else if(transmit[0] > 560){
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        //Map joystick values to range between 0 to 255 to control DC motors
        MotorSpeed1 = map(transmit[0], 560, 1023, 0, 255); 
        //Send data to driver motor
        analogWrite(enA, MotorSpeed1);
      //If joystick is in the middle, set motor speed to 0
      }else{
        MotorSpeed1 = 0;
        analogWrite(enA, MotorSpeed1);
      }

      //Read transmission data to control Left Motor 2
      //Works the exact same way as Motor 1 code above
      if(transmit[1] < 450){
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        transmit[1] = transmit[1] - 450; // This produces a negative number
        transmit[1] = transmit[1] * -1;  // Make the number positive
        MotorSpeed2 = map(transmit[1], 0, 450, 0, 255);
        analogWrite(enB, MotorSpeed2);
      }else if(transmit[1] > 560){
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        MotorSpeed2 = map(transmit[1], 560, 1023, 0, 255); 
        analogWrite(enB, MotorSpeed2);
      }else{
        MotorSpeed2 = 0;
        analogWrite(enB, MotorSpeed2);
      }
  }
    
    delay(15);
}
  
