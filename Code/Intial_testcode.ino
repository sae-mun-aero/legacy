#include <Servo.h>
//Declaring Variables
byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
int receiver_input_channel_1, receiver_input_channel_2, receiver_input_channel_3, receiver_input_channel_4;
unsigned long timer_1, timer_2, timer_3, timer_4;
int throatle = 7;
int pitch = 3;
int yaw = 4;
int Roll_1 = 5;
int Roll_2 = 6;

double throatle_value = 0;
double Roll_1_value = 0;
double Roll_2_value = 0;
double pitch_value = 0;
double yaw_value = 0;

Servo throatle_servo; 
Servo pitch_servo; 
Servo yaw_servo; 
Servo Roll_1_servo; 
Servo Roll_2_servo; 

//Setup routine
void setup(){
  //Arduino (Atmega) pins default to inputs, so they don't need to be explicitly declared as inputs
  PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);  // set PCINT0 (digital input 8) to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT1);  // set PCINT1 (digital input 9)to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT2);  // set PCINT2 (digital input 10)to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT3);  // set PCINT3 (digital input 11)to trigger an interrupt on state change
  Serial.begin(9600); 
  throatle_servo.attach(throatle);
  pitch_servo.attach(pitch);
  yaw_servo.attach(yaw);
  Roll_1_servo.attach(Roll_1);
  Roll_2_servo.attach(Roll_2);
}

//Main program loop
void loop(){
  //print_signals();
  throatle_servo.write(int (throatle_value));
  pitch_servo.write(int(Roll_1_value));
  yaw_servo.write(int(Roll_2_value));
  Roll_1_servo.write(int(pitch_value));
  Roll_2_servo.write(int(yaw_value));
  
  Serial.print("Yaw:");
  Serial.print(yaw_value);
  
  Serial.print("  Gas:");
  Serial.print(throatle_value);
  
  Serial.print("  Pitch:");
  Serial.print(pitch_value);
  
  Serial.print("  Roll:");
  Serial.println(Roll_1_value);
}

//This routine is called every time input 8, 9, 10 or 11 changed state
ISR(PCINT0_vect){
  //Channel 1========================================= 8 -- Yaw
  if(last_channel_1 == 0 && PINB & B00000001 ){         //Input 8 changed from 0 to 1
    last_channel_1 = 1;                                 //Remember current input state
    timer_1 = micros();                                 //Set timer_1 to micros()
  }
  else if(last_channel_1 == 1 && !(PINB & B00000001)){  //Input 8 changed from 1 to 0
    last_channel_1 = 0;                                 //Remember current input state
    receiver_input_channel_1 = micros() - timer_1;      //Channel 1 is micros() - timer_1
    yaw_value = (receiver_input_channel_1 - 1000);
    yaw_value = (yaw_value / 1000) * 255;
  }
  //Channel 2========================================= 9 -- Gas
  if(last_channel_2 == 0 && PINB & B00000010 ){         //Input 9 changed from 0 to 1
    last_channel_2 = 1;                                 //Remember current input state
    timer_2 = micros();                                 //Set timer_2 to micros()
  }
  else if(last_channel_2 == 1 && !(PINB & B00000010)){  //Input 9 changed from 1 to 0
    last_channel_2 = 0;                                 //Remember current input state
    receiver_input_channel_2 = micros() - timer_2;      //Channel 2 is micros() - timer_2
    throatle_value = (receiver_input_channel_2 - 1000);
    throatle_value = (throatle_value / 1000) * 255;
  }
  //Channel 3========================================= 10 -- Pitch 
  if(last_channel_3 == 0 && PINB & B00000100 ){         //Input 10 changed from 0 to 1
    last_channel_3 = 1;                                 //Remember current input state
    timer_3 = micros();                                 //Set timer_3 to micros()
  }
  else if(last_channel_3 == 1 && !(PINB & B00000100)){  //Input 10 changed from 1 to 0
    last_channel_3 = 0;                                 //Remember current input state
    receiver_input_channel_3 = micros() - timer_3;      //Channel 3 is micros() - timer_3
    pitch_value = (receiver_input_channel_3 - 1000);
    pitch_value = (pitch_value / 1000) * 255;
  }
  //Channel 4========================================= 11 -- Roll
  if(last_channel_4 == 0 && PINB & B00001000 ){         //Input 11 changed from 0 to 1
    last_channel_4 = 1;                                 //Remember current input state
    timer_4 = micros();                                 //Set timer_4 to micros()
  }
  else if(last_channel_4 == 1 && !(PINB & B00001000)){  //Input 11 changed from 1 to 0
    last_channel_4 = 0;                                 //Remember current input state
    receiver_input_channel_4 = micros() - timer_4;      //Channel 4 is micros() - timer_4
    Roll_1_value = (receiver_input_channel_4 - 1000);
    Roll_1_value = (Roll_1_value / 1000) * 255;
    Roll_2_value = 256 - Roll_1_value;
  }
}
//Subroutine for displaying the receiver signals
void print_signals(){
  Serial.print("Yaw:");
  if(receiver_input_channel_1 - 1480 < 0)Serial.print("<<<");
  else if(receiver_input_channel_1 - 1520 > 0)Serial.print(">>>");
  else Serial.print("-+-");
  Serial.print(receiver_input_channel_1);
  
  Serial.print("  Gas:");
  if(receiver_input_channel_2 - 1480 < 0)Serial.print("^^^");
  else if(receiver_input_channel_2 - 1520 > 0)Serial.print("vvv");
  else Serial.print("-+-");
  Serial.print(receiver_input_channel_2);
  
  Serial.print("  Pitch:");
  if(receiver_input_channel_3 - 1480 < 0)Serial.print("vvv");
  else if(receiver_input_channel_3 - 1520 > 0)Serial.print("^^^");
  else Serial.print("-+-");
  Serial.print(receiver_input_channel_3);
  
  Serial.print("  Roll:");
  if(receiver_input_channel_4 - 1480 < 0)Serial.print("<<<");
  else if(receiver_input_channel_4 - 1520 > 0)Serial.print(">>>");
  else Serial.print("-+-");
  Serial.println(receiver_input_channel_4);
}
