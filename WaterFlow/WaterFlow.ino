#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_PCD8544.h>
#define btnNONE   0
#define btnUP     1
#define btnRIGHT  2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

char number[11]="00000000000";
int count = 3;
char sms[150],text;
char pass[6] = "123456";
int readbtn(){
    delay(100);
    int adc_key_in = analogRead(1);
    if (adc_key_in > 1000) return btnNONE; 
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 195)  return btnUP; 
    if (adc_key_in < 380)  return btnDOWN; 
    if (adc_key_in < 555)  return btnLEFT; 
    if (adc_key_in < 790)  return btnSELECT;   
    return btnNONE;  // when all others fail, return this...
}

float distance1(){  
    float duration=0,distance=0,avg=0; 
    for(int i=0;i<count;i++){
        digitalWrite(11, LOW);
        delayMicroseconds(2);
        digitalWrite(11, HIGH);     
        delayMicroseconds(30);              
        digitalWrite(11, LOW);  
        duration = pulseIn(10, HIGH);
        distance = ((duration / 2) * 0.0345);
        avg+=distance;
        delay(300);
    }
    avg/=count;
    return avg;   
}

float distance2(){  
    float duration=0,distance=0,avg=0; 
    for(int i=0;i<count;i++){
        digitalWrite(9, LOW);
        delayMicroseconds(2);
        digitalWrite(9, HIGH);     
        delayMicroseconds(30);              
        digitalWrite(9, LOW);  
        duration = pulseIn(8, HIGH);
        distance = ((duration / 2) * 0.0345);
        avg+=distance;
        delay(300);
    }
    avg/=count;
    return avg;   
}

void change_number(){
      for(int co=20;co<31;co++){
            number[co-20]=EEPROM.read(co);
      }
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("Change Number");
      display.display();
      display.setCursor(0,20);
      byte loc = 0;
      while(1){
            display.clearDisplay();
            display.setCursor(0,0);
            display.print("Change Number");
            display.display();
            display.setCursor(0,20);
            for(int i=0;i<11;i++){
                display.print(number[i]);
                display.display();
            }
            display.setCursor(loc,30);
            display.print("^");
            display.display();
            while(readbtn()==btnNONE){}
            if(readbtn()==btnRIGHT){
                if(loc<60) loc+=6;  
            }
            if(readbtn()==btnLEFT){
                if(loc>0) loc-=6;  
            }
            if(readbtn()==btnUP){
                number[loc/6]++;
                if(number[loc/6]>57) number[loc/6]=48;   
            }
            if(readbtn()==btnDOWN){
                number[loc/6]--;
                if(number[loc/6]<48) number[loc/6]=57;   
            }  
            if(readbtn()==btnSELECT){
                  display.clearDisplay();
                  display.setCursor(0,30);
                  display.print("Number Changed!");
                  display.display();
                  break;  
            }
       }
       while(readbtn() != btnNONE){}
       for(int co=20;co<31;co++){
            EEPROM.update(co,number[co-20]);
       }

}

void send_msg(){
      display.clearDisplay();
      display.print("Sending msg...");
      display.display();
      Serial.print("AT");
      delay(500);
      Serial.write(0xD);
      delay(500);
      Serial.print("AT+CMGF=1");
      delay(500);
      Serial.write(0xD);
      delay(900);
      Serial.print("AT+CMGS=");
      delay(800);
      Serial.write(0x22);
      for(int i=0;i<11;i++){
            Serial.print(number[i]);
      }
      delay(500);
      Serial.write(0x22);
      delay(200);
      Serial.write(0xD);
      delay(800);
      Serial.print("Dist1: ");
      delay(300);
      Serial.print(distance1());
      delay(300);
      Serial.print("\nDist2: ");
      delay(300);
      Serial.print(distance2());
      delay(300);
      Serial.write(0x1A);
      delay(800);
      display.clearDisplay();
      display.print("msg sent!");
      delay(2000);
}
void change_pass(){
  for(int co=40;co<46;co++){
            pass[co-40]=EEPROM.read(co);
      }
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("Change Password");
      display.display();
      display.setCursor(0,20);
      byte loc = 0;
      while(1){
            display.clearDisplay();
            display.setCursor(0,0);
            display.print("Change Pass");
            display.display();
            display.setCursor(0,20);
            for(int i=0;i<6;i++){
                display.print(pass[i]);
                display.display();
            }
            display.setCursor(loc,30);
            display.print("^");
            display.display();
            while(readbtn()==btnNONE){}
            if(readbtn()==btnRIGHT){
                if(loc<30) loc+=6;  
            }
            if(readbtn()==btnLEFT){
                if(loc>0) loc-=6;  
            }
            if(readbtn()==btnUP){
                pass[loc/6]++;
                if(pass[loc/6]>57) pass[loc/6]=48;   
            }
            if(readbtn()==btnDOWN){
                pass[loc/6]--;
                if(pass[loc/6]<48) pass[loc/6]=57;   
            }  
            if(readbtn()==btnSELECT){
                  display.clearDisplay();
                  display.setCursor(0,30);
                  display.print("Pass Changed!");
                  display.display();
                  break;  
            }
       }
       while(readbtn() != btnNONE){}
       for(int co=40;co<46;co++){
            EEPROM.update(co,pass[co-40]);
       }

  
  }

void setup() {
      int gh;
      Serial.begin(9600);
      pinMode(10,INPUT);//echo of sensor 1
      pinMode(11,OUTPUT);//triger 1
      pinMode(8,INPUT);//echo of sensor 2//green
      pinMode(9,OUTPUT);//triger 2//white
      display.begin();
      display.setContrast(60);
      display.clearDisplay();
      display.setCursor(20,10);
      display.print("Welcome");
      display.setCursor(10,30);
      display.print("Starting...");
      display.display();
      Serial.print("AT+CNMI=2,2,0,0,0");
      delay(100);
      Serial.write(0xD);
      delay(600);
      Serial.print("AT+CSMP=17,167,0,0");
      delay(100);
      Serial.write(0xD);
      Serial.print("AT+CMGF=1");
      delay(100);
      Serial.write(0xD);
      Serial.print("AT+CMGR=1");
      delay(100);
      Serial.write(0xD);
      Serial.print("ATE0");
      delay(100);   
      Serial.write(0xD);
}

void loop() {
      int gh=0;
      while(readbtn() != btnSELECT){
          display.clearDisplay();
          display.setCursor(0,0);
          display.print("dist1: ");
          display.setCursor(40,0);
          display.print(distance1());
          display.setCursor(0,10);
          display.print("dist2: ");
          display.setCursor(40,10);
          display.print(distance2());
          display.setCursor(30,30);
          display.print("MENU");
          display.display();
          //**********************************
          char rec_pass[6];
          int c_sms=0;
          if(Serial.available()>40){ 
              while(Serial.available()){
                  text=Serial.read();
                  sms[c_sms]=text;
                  c_sms++;
              }
              display.clearDisplay();
              display.setCursor(0,20);
              display.print("Msg Recived");
              display.display();
              delay(3000);  
              Serial.println("this is Password");
              for(int i=18;i<150;i++){
                  Serial.print(sms[i]);
                  if(i>17 && i<29) number[i-17] = sms[i];
                  if(i>56 && i<63) rec_pass[i-57] = sms[i];
              }
              Serial.println("");
              Serial.println("End ");
              Serial.print("AT+CMGDA=");
              delay(100);
              Serial.write(0x22);
              delay(100);
              Serial.print("DEL ALL");
              delay(100);
              Serial.write(0x22);
              delay(100);
              Serial.write(0xD);
              Serial.flush();
              Serial.println("pass sina is: ");
              for(int i=0;i<6;i++){
                  Serial.print(rec_pass[i]);  
              }
              for(int co=40;co<46;co++){
                pass[co-40]=EEPROM.read(co);
              }
              Serial.println("");
              bool pass_s=true;
              for(int j=0;j<6;j++){
                if(pass[j]!=rec_pass[j]){
                  pass_s=false;
                  break;
                  }
              }
              if(pass_s) {
                
                send_msg();
                for(int i=0;i<150;i++){
                  sms[i] = '0';
                  }
                }
              }
      }
      display.clearDisplay();
      display.display();
      while(readbtn() == btnSELECT){};
      byte menu=1;
      while(1){
          display.clearDisplay();
          if(menu==1)display.print("*");
          display.setCursor(5,0);
          display.print("Change Number");
          display.setCursor(0,10);
          if(menu==2)display.print("*");
          display.setCursor(5,10);
          display.print("Send MSG");
          display.setCursor(0,20);
          if(menu==3)display.print("*");
          display.setCursor(5,20);
          display.print("Change Pass");
          display.print(readbtn());
          display.display();
          //while(readbtn()==btnNONE){}
          if(readbtn()==btnDOWN){
              menu++;
              if(menu>3) menu=1; 
              while(readbtn()!=btnNONE){};
          }
          if(readbtn()==btnUP){
              menu--;
              if(menu<1) menu=3; 
              while(readbtn()!=btnNONE){};
          }
          if(readbtn()==btnSELECT){
              break; 
              while(readbtn()!=btnNONE){};
          }
          if(readbtn()==btnLEFT){
              menu = 4;
              break; 
              while(readbtn()!=btnNONE){};
          }
      }
      while(readbtn() != btnNONE){};
      if(menu == 2) {
              for(int co=20;co<31;co++){
            number[co-20]=EEPROM.read(co);
      }

        send_msg();
      }
      if(menu == 1) change_number();
      if(menu == 3) change_pass();
      
}

