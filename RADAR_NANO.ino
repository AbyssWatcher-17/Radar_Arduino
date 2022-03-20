 #include "Arduino.h"
 #include <SPI.h>
 #include <RF24.h>
 #include <Wire.h> 
 #include <LiquidCrystal_I2C.h>
 #include <LcdBarGraphRobojax.h>

    LiquidCrystal_I2C lcd(0x27, 16, 2);

     LcdBarGraphRobojax lbg(&lcd, 6, 0, 1);
// NANO
int change_bar=90;
int distance;
int freq_R;
int freq_G;


struct data {
long ch1;
long ch2;
long ch3;
long ch4;
};  data package_data;

 struct  Data_received {
  float ch1;
  float ch2;
  float ch3;
//  float ch4;

 
}; Data_received data_received;


RF24 radio(9, 10);
byte addresses[][6] = {"1Node", "2Node"};


void setup() {
  
  Serial.begin(9600); 
  lcd.init();// if it gives error try, lcd.begin();
  lcd.backlight();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(124);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
 
}

void led_func(int distance,int freq_R,int freq_G){
 // Serial.println("LED_FUNC started");
   if(distance<=10000) 
   {
    
    if(freq_R<freq_G && freq_R<=750){
     // Serial.println("RED");
    analogWrite(5,250);
    analogWrite(6,0);
 
    }
    if(freq_G<freq_R&& freq_G<=750){
    //Serial.println("GREEN");
    analogWrite(5,0);
    analogWrite(6,1);
   
    }
  
   }
   else{
    analogWrite(5,0);
    analogWrite(6,0);
      }
 }

void loop() {
  
  if(analogRead(A2)>600){
    change_bar+=1;
    }
  if(analogRead(A2)<400){
    change_bar-=1;
    }
   if(change_bar>180){ change_bar =180;}
   if(change_bar<0){ change_bar =0;}
  //-------------------------------DUAL_COM-----------------------------
   
  
   package_data.ch1 = change_bar;

   
   Serial.print("change_bar-> ");
   Serial.print(change_bar);
   Serial.println(" ");
   
   package_data.ch2 = 100;  
   package_data.ch3 = 100; 

       
  radio.stopListening();  
  if (!radio.write(&package_data , sizeof(data))) {
                       
        // HATA001*--------  
    }
  radio.startListening(); 
  unsigned long started_waiting_at = millis();
  while ( ! radio.available() ) 
  {
    if (millis() - started_waiting_at > 3 ) 
       {
 
        //HATA002*--------  
      return;
       }
  }
  radio.read(&data_received, sizeof(Data_received));

   
        lcd.setCursor(0,0);
        lcd.print("distance ->");
        lcd.print(data_received.ch1);
        lbg.drawValue(map(analogRead(A2),0,1023,0,680), 250);
    
    distance = data_received.ch1;
    freq_R = data_received.ch2;
    freq_G = data_received.ch3;
    
    //led=on & led=off
   
   led_func(distance,freq_R,freq_G);

    
     //Serial.println("distance -> ");
     //Serial.print(distance);
    
  //  delay(2);
    
    
 //-------------------------------DUAL_COM----------------------------- 

}

  /*
   Serial.print(data_received.ch1,6);
   Serial.print("*");
   Serial.print(data_received.ch2,6);
   Serial.print("*");
   Serial.print(data_received.ch3);
   Serial.print("*");
   Serial.print(data_received.ch4);
   Serial.print("*");
   Serial.print(data_received.ch5);
   Serial.print("*");
   Serial.print(abs(data_received.ch6));
   Serial.print("*");
   Serial.print(abs(data_received.ch7));
   Serial.print("*");
   Serial.print(data_received.ch8);
  */
