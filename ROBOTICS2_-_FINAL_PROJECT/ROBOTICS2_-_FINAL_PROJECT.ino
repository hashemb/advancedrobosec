
//
//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;

#define BLYNK_PRINT Serial
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#define SS_PIN 2  
#define RST_PIN 0 
#define piezo 16
#define ServoMotorpin 5
#include <SPI.h>
#include <MFRC522.h>
Servo servo1;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

char auth[] = "c551e73c41374f8cb5f1b24c23ddb898";

// Your WiFi credentials.x
// Set password to "" for open networks.
//char ssid[] = "HilalNet_76060306 Tamam";
//char pass[] = "Hash00000";

char ssid[] = "Hashem's";
char pass[] = "0000hothot";

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  Blynk.begin(auth, ssid, pass);
  mfrc522.PCD_Init();   // Initiate MFRC522
  analogWrite(ServoMotorpin, 512);
  analogWriteFreq(50); 
  servo1.attach(5); 
      servo1.write(1);

}
void loop() 
{
  Blynk.run();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "E3 BA 53 2D") //change UID of the card that you want to give access
  {
    tone( piezo, 5000, 100);
    delay(160);
    tone( piezo, 5000, 50);  
    servo1.write(180);
    Blynk.notify("Your RFID has been detected, WELCOME!");
    Serial.println(" Access Granted ");
    
    delay(3000);
    servo1.write(1);

    Serial.println();
    statuss = 1;
  }
  
  else   {
   Blynk.email("hashe6m@gmail.com", "ESP8266 Alert", "Wrong RFID trying to enter");
    Blynk.notify("Security Alert - Wrong RFID trying to enter");
    Serial.println(" Access Denied ");
    siren();
    siren();
    siren();
    delay(3000);
  }
}


 void siren()//This function will make the alarm sound using the piezo buzzer
{
  
for(int hz = 440; hz < 1000; hz++){
    tone(piezo, hz, 50);
    delay(5);
  }
  noTone(piezo);

}
