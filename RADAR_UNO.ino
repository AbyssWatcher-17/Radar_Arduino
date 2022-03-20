#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Servo.h>

#define S2 4
#define S3 2
#define sensorOut 3
// UNO
Servo servo;

 long distance;
 long time_d;

 
int frequency_G = 0;//Green
int frequency_R= 0;//Red

 
struct  Data_received {
  long ch1;
  long ch2;
  long ch3; 
  long ch4;

};Data_received data_received;
struct data {
  float ch1;
  float ch2;
  float ch3;
  float ch4;


};data package_data;

     int x;
     int y;
     int z;
     int m;
          
RF24 radio(9, 10);
byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(9600);
   pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
   pinMode(5, OUTPUT);
   pinMode(6, INPUT);
  servo.attach(7); //servo
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(124);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.startListening();
  Wire.begin();
  
}

void loop() {

   digitalWrite(5, HIGH);
   delayMicroseconds(5);
   digitalWrite(5, LOW);
   time_d = pulseIn(6, HIGH);
   distance = time_d / 2 / 29.1; //
   //if(distance > 200)
   //distance = 200;
   Serial.println(distance);

   digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency_R= pulseIn(sensorOut, LOW);
  Serial.print("R= ");//printing name
  Serial.print(frequency_R);//printing RED color frequency
  Serial.print("   ");

  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency_G= pulseIn(sensorOut, LOW);
  Serial.print("G= ");//printing name
  Serial.println(frequency_G);//printing RED color frequency
  
   package_data.ch1 = distance;
   package_data.ch2 = frequency_R; 
   package_data.ch3 = frequency_G;


 
  if (radio.available()) 
    {while (radio.available()) 
      {
      radio.read(&data_received, sizeof(Data_received));
      }
    radio.stopListening(); 
    radio.write(&package_data , sizeof(data));
    radio.startListening();

      x = data_received.ch1;
      y = data_received.ch2;
      z = data_received.ch3;
   //   m = data_received.ch4;
      
     
      Serial.println(x);

     servo.write(x);

      
   
        
      

}
}
