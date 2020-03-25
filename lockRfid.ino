/* Lock system using RFID
 * By Samaksh Sethi
 * 
 * Execution procedure:
 * 1. The first card to be recognized gets assigned 0 index and is termed as the "master card"
 * 2. The following cards have to be used after first scanning the master card, after which 2 things can happen
 *    (i) Add-> If the card does not exist in the index already, it is added and authorized for future scans
 *    (ii) Delete-> If the card does exists in the index already, it is deleted and unauthorized for future scans
 *    
 * NOTE- The master card cannot be deleted without using the RST pin on the board.
 */
 
#include <SPI.h>      //Required for serial communication with MFRC522
#include <MFRC522.h>  //Standard library for RFID
#include <Servo.h>

/*
 * By default, for UNO,
 * RST          9
 * SDA(SS)      10
 * MOSI         11
 * MISO         12
 * SCK          13
 */
 
#define RST_PIN   9
#define SS_PIN    10

boolean successRead=0;
String tagID="";
char* card[100]={};   //Consists of all cards UID
int t=1;              //Keeps track of card index
byte readCard[4]={};  //Stores one byte of the 4 bytes from the UID
int flag=1;           //Used to only check if theres a wrong card recognized

#define unlockAngle 0   //Change the value according to where the server is positioned to unlock the door
#define lockAngle 180   //Change the value according to where the server is positioned to lock the door

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo;

void setup() {
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(8);    //Servo data pin is 8
  myServo.write(unlockAngle);
  Serial.begin(9600);

  checkCard();
  card[0]=strdup(tagID.c_str());    //strdup-> Gives a pointer pointing to the (null-terminated) string
                                    //c_str()-> Converts the string to a null-terminated string
  Serial.println("Master Card Read");
}

void loop() {
  checkCard();
  if(tagID==card[0]){
    Serial.println("Ready to add/delete card");
    checkCard();
    for(int j=0;j<100;j++){
      if(tagID==card[j]){
        card[j]="";
        Serial.println("Card deleted");
        return;
      }
    }
    card[t]=strdup(tagID.c_str());
    t++;
    Serial.println("Card added");
  }

  for(int j=1;j<100;j++){
    if(tagID==card[j]){
      Serial.println("Unlocked");
      myServo.write(180);
      delay(3000);                      //The door is unlocked for 3 seconds by default
      myServo.write(0);
      flag=0;
      return;
    }
  }
  if(flag=1){
    Serial.println("Try Again");      ///Error if an unregisteered card is scanned
  }
  flag=1;
}

void checkCard(){     //
  while(!successRead){
    successRead=getID();
  }
  successRead=false;
}

int getID(){    //Waits for a new PICC to be recognised, gets the UID and saves it to a global variable, tagID  
  if(!mfrc522.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()){
    return 0;
  }
  tagID="";
  for(int i=0;i<4;i++){   //instead of 4, mrfc522.uid.size can be used
    readCard[i]=mfrc522.uid.uidByte[i];
    tagID.concat(String(readCard[i],HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();   //Stops reading
  return 1;
}
