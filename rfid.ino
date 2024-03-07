#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include<Servo.h>

Servo myServo;

int pos = 0;

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
String MasterTag = "93 C6 20 AA";  // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16,2); //Parameters: (rs, enable, d4, d5, d6, d7)

void setup()
{
  // Initiating
  SPI.begin(); // SPI bus
  myServo.attach(3);
  mfrc522.PCD_Init(); // MFRC522
  lcd.begin(16, 2); // LCD screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome to car");
  lcd.setCursor(0,1);
  lcd.print("Car Locking System");
  delay(2000);
  myServo.write(0);
  delay(1000);
  myServo.write(90);
  delay(1000);
  myServo.write(0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Access Control ");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card>>");
}

void loop()
{
 
  //Wait until new tag is available
  while (getID())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
   
    if (tagID == MasterTag)
    {
      myServo.write(90);
      lcd.print(" Access Granted!");
      // You can write any code here like opening doors, switching on a relay, lighting up an LED, or anything else you can think of.
    }
    else
    {
      lcd.print(" Access Denied!");
      myServo.write(pos);
    }
   
      lcd.setCursor(0, 1);
      lcd.print(" ID : ");
      lcd.print(tagID);
     
    delay(2000);

    lcd.clear();
    lcd.print(" Access Control ");
    lcd.setCursor(0, 1);
    lcd.print("Scan Your Card>>");
  }
}

//Read new tag if available
boolean getID()
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
