#define REMOTEXY_MODE__ESP32CORE_WIFI_POINT
#include <WiFi.h>
#include <RemoteXY.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <EEPROM.h>



int ButtonValue = 0;
int Button = 22;
int LED = 2;

byte ID[4] = {25, 231, 60, 195};
int i = 0;


int adres[40];
int a = 0;
byte userIdList[41][4];
static int iter;
boolean iterExceeded=false;
boolean checkUser=false;
byte deneme=5;


#define REMOTEXY_WIFI_SSID "RD_door"
#define REMOTEXY_WIFI_PASSWORD "EndArge4242"
#define REMOTEXY_SERVER_PORT 6377


#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =  
{ 255, 1, 0, 0, 0, 69, 0, 16, 113, 1, 129, 0, 10, 68, 45, 5, 130, 69, 110, 100,
  195, 188, 115, 116, 114, 105, 121, 101, 108, 32, 65, 82, 45, 71, 69, 0, 10, 48, 20, 22,
  25, 25, 183, 148, 31, 79, 78, 0, 31, 79, 70, 70, 0, 129, 0, 14, 75, 38, 6, 146,
  72, 97, 121, 196, 177, 114, 108, 196, 177, 32, 79, 108, 115, 117, 110, 0
};

struct {

  uint8_t pushSwitch_1;

  uint8_t connect_flag;

} RemoteXY;
#pragma pack(pop)


#define SS_PIN 4
#define RST_PIN 27
MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo myservo;

int pos = 0;
#if defined(ARDUINO_ESP32S2_DEV)
int servoPin = 17;
#else
int servoPin = 17;
#endif


void setup()
{
  Serial.begin(9600);
  pinMode(Button, INPUT);
  pinMode(LED, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  RemoteXY_Init ();
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 1000, 2000);
  EEPROM.begin(200);
  pinMode(15,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(21,OUTPUT);
  
  
}

void loop()
{

//Servo Test

//  pos = 0;
//    myservo.write(pos);
//    delay(500);
//    pos =45 ;
//    myservo.write(pos);
//    delay(500);
//    pos = 90;
//    myservo.write(pos);
//    delay(500);
//    pos = 135;
//    myservo.write(pos);
//    delay(500);
//    pos = 170;
//    myservo.write(pos);
//    delay(500);

ButtonValue = digitalRead(Button);
  
  if (RemoteXY.pushSwitch_1 != 0)
  {
    //Serial.println("Giris Basarili");
    //delay(250);
  digitalWrite(15,HIGH);
  delay(250);
    pos = 175;
    myservo.write(pos);
    delay(500);
  digitalWrite(15,LOW);
    RemoteXY.pushSwitch_1 = 0;

  }
  
  else if (RemoteXY.pushSwitch_1 == 0) {
    pos = 90;
    myservo.write(pos);
    delay(100);
  }
  RemoteXY_Handler ();
/*if ( ! mfrc522.PICC_IsNewCardPresent()){
   return;
}
 if ( ! mfrc522.PICC_ReadCardSerial()){
    return;
} */  
  
  
  if (ButtonValue == 1) {
    Serial.print(EEPROM.read(31));
    //butonflag=1;
     digitalWrite(3, HIGH);
     
     Serial.println(EEPROM.read(4*0));
      Serial.println(EEPROM.read(4*0+1));
      Serial.println(EEPROM.read(4*0+2));
      Serial.println(EEPROM.read(4*0+3));

     
   if( ! mfrc522.PICC_IsNewCardPresent())
  {   
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()){      
    return;  
  }


    for(int i=0; i<4; i++){
      if(iter==40){
        iterExceeded=true;
        iter=0;
        userIdList[iter][i]=mfrc522.uid.uidByte[i];
      }
      userIdList[iter][i]=mfrc522.uid.uidByte[i];
      
      Serial.println("a");
    }
   iter++;
      
      
   for(int i=0; i<40; i++){   
       
      
    if(EEPROM.read(4*i)==255 && EEPROM.read(4*i+1)==255 && EEPROM.read(4*i+2)==255 && EEPROM.read(4*i+3)==255){
      EEPROM.write(4*i, userIdList[iter-1][0]);
      EEPROM.commit();
      EEPROM.write(4*i+1, userIdList[iter-1][1]);
      EEPROM.commit();
      EEPROM.write(4*i+2, userIdList[iter-1][2]);
      EEPROM.commit();
      EEPROM.write(4*i+3, userIdList[iter-1][3]);
      EEPROM.commit();

      Serial.print(EEPROM.read(4*i));
      Serial.print(EEPROM.read(4*i+1));
      Serial.print(EEPROM.read(4*i+2));
      Serial.println(EEPROM.read(4*i+3));
      break;
    }
   }
   
       mfrc522.PICC_HaltA();

    
  /* 
    mfrc522.uid.uidByte[0]
    mfrc522.uid.uidByte[1] 
    mfrc522.uid.uidByte[2] 
    mfrc522.uid.uidByte[3]
  */
  }
  
  if (ButtonValue == 0) {
    digitalWrite(3, LOW);
     if( ! mfrc522.PICC_IsNewCardPresent())
  {   
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()){      
    return;  
  }
  /*
  if(iterExceeded){
    for(int i =0; i<40; i++){
      checkUser=true;
      for(int j=0; j<4; j++){
        if(mfrc522.uid.uidByte[j] != userIdList[i][j]){ 
            checkUser=false;
        } 
      }
      
      if(checkUser){
          //motor hareket
           delay(250);
    pos = -25;
    myservo.write(pos);
    delay(500);
    pos = 90;
    myservo.write(pos);
    delay(1000);
        } 
    }
  }else{
    for(int i =0; i<iter; i++){
      checkUser=true;
      for(int j=0; j<4; j++){
        if(mfrc522.uid.uidByte[j] != userIdList[i][j]){ 
           checkUser=false;
        } 
      }
      
      if(checkUser){
          //motor hareket
    delay(250);
    pos = -25;
    myservo.write(pos);
    delay(500);
    pos = 90;
    myservo.write(pos);
    delay(1000);
        } 
    }  
  }
}//if button ==0
*/


checkUser=false;
  for(int i=0; i<40; i++){
    if(EEPROM.read(4*i) == mfrc522.uid.uidByte[0]){
      if(EEPROM.read(4*i+1) == mfrc522.uid.uidByte[1]){
        if(EEPROM.read(4*i+2) == mfrc522.uid.uidByte[2]){
          if(EEPROM.read(4*i+3) == mfrc522.uid.uidByte[3]){
            checkUser=true;
            break;
          }
        }
      }
    }
  }

if(checkUser){

  digitalWrite(15,HIGH);
  delay(250);
    pos = 175;
    myservo.write(pos);
    delay(500);
  digitalWrite(15,LOW);

}
if(checkUser != 1){
  digitalWrite(21, HIGH);
  delay(1500);
  digitalWrite(21, LOW);
  }

  
  
}
  

      mfrc522.PICC_HaltA();

    
   /* delay(250);
    pos = -25;
    myservo.write(pos);
    delay(500);
    pos = 90;
    myservo.write(pos);
    delay(1000);
  */
  
 /*else{                                 //Yetkisiz girişte içerideki komutlar çalıştırılır.
      
("Yetkisiz Kart");
      ekranaYazdir();
    }
*/
}









void ekranaYazdir(){
  Serial.print("ID Numarasi: ");
  for(int sayac = 0; sayac < 4; sayac++){
    Serial.print(mfrc522.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}

  
  
