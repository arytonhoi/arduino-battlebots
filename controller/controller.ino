/*
* Code for controller
* By: Aryton Hoi
*/
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//Setup NRF Wireless Communication Module
RF24 radio(7,8); // CE, CSN
//Setup radio address
//Change address to "00002" for second robot
const byte address[6] = "00001"; 

//Setup joystick pins
const int joy1 = A0;
const int joy2 = A1;
//Setup joystick position values
int joy1Pos;
int joy2Pos;

//Setup button pins
const int button1 = A2;
const int button2 = A3;
//Setup button values
int button1State;
int button2State;

//Setup vibrator pins
const int vibrator = 6;
const int vibrator2 = 9;
//Setup vibrator values
int vibstate;
int vib2state;

void setup()
{
  //Serial communication for debuging
  //Serial.begin(9600);
  
  //Start wireless communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  //Set NRF module as transmitter and don't receive data
  radio.stopListening();

  //Setup button pins
  pinMode(button1, INPUT); 
  pinMode(button2, INPUT);
  //Setup vibrator pins
  pinMode(vibrator, OUTPUT);
  pinMode(vibrator2, OUTPUT);
}

void loop()
{
  //Read joystick positions
  joy1Pos = analogRead(joy1);
  joy2Pos = analogRead(joy2);

  //Read button states
  button1State = digitalRead(button1);
  button2State = digitalRead(button2);

  //Vibration motor code
  if(button1State == 1){
    digitalWrite(vibrator, HIGH);
  }else{
    digitalWrite(vibrator, LOW);
  }
  button2State = digitalRead(button2);
  if(button2State == 1){
    digitalWrite(vibrator2, HIGH);
  }else{
    digitalWrite(vibrator2, LOW);
  }
  
  //Format and transmit input data through NRF wireless module
  //Transmit joystick and button readings
  int transmit[4] = {joy1Pos, joy2Pos, button1State, button2State};
  radio.write(&transmit, sizeof(transmit));

  //Print transmission values for debugging
  /*
  Serial.print(transmit[0]);
  Serial.print(", ");
  Serial.print(transmit[1]);
  Serial.print(", ");
  Serial.print(transmit[2]);
  Serial.print(", ");
  Serial.println(transmit[3]);
  */
  
  delay(15);
}

