
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

//res ==> pin 9
//MISO ==> 12
//MOSI ==> 11
//SCK ==> 13
//SDA ==> 10

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//Photoresistance
const char AN_PHOTORESISTANCE = 0;
const char DOUT_LED = 3;
int offset = 285;


int positionDuServo=90;
Servo monServo;
//servo attach pin 8

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
   
  monServo.attach(8);
  monServo.write(positionDuServo);

  pinMode(DOUT_LED,OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  analogWrite(DOUT_LED,LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
}
 
void loop()
{

  int valeurSurA0 = analogRead(AN_PHOTORESISTANCE);
  int intensiteLED = 720 - valeurSurA0;
 
  intensiteLED = intensiteLED - offset;
 
  if(intensiteLED<0)
    {intensiteLED=0;}
  
  analogWrite(DOUT_LED, intensiteLED);
  delay(10);

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
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "50 A0 63 56") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    
    digitalWrite(7, HIGH);
    delay(100);
    digitalWrite(7, LOW);
    digitalWrite(2, HIGH);
    for(int i=90;i<=180;i++){
          positionDuServo = i;
          monServo.write(positionDuServo);
          delay(10);
        }
        delay(2000);
        digitalWrite(2, LOW);
        for(int i=180;i>=90;i--){
          positionDuServo = i;
          monServo.write(positionDuServo);
          delay(10);
        }
    Serial.println();
    delay(1100);
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    delay(1000);
    digitalWrite(7, LOW);
    digitalWrite(4, LOW);
  }


  
    
    
}
