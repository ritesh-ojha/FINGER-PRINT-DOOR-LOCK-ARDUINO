#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

uint8_t getFingerprintEnroll(uint8_t id);

String pass ="" ;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'}
  ,
  { '4','5','6','B'}
  ,
  {'7','8','9','C'}
  ,
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 10, 11}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(15,14,16,17,18,19); //Parameters: (rs, enable, d4, d5, d6, d7)

int getFingerprintIDez();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int led = 13;
int bell = 12;

void setup() {
		
		
 Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(led,OUTPUT);
  pinMode(bell,OUTPUT);
  digitalWrite(led, LOW);
  digitalWrite(bell, LOW);
  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
       Serial.println("Found fingerprint sensor!");
  }
  else {
       Serial.println("Did not find fingerprint sensor..");
    lcd.setCursor(0, 1);
    lcd.write("  FP Error...");
    password();
  }
   // Serial.println("Waiting for valid finger...");
  pinMode(led, OUTPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("  FINGER PRINT");
    Serial.println("WELLCOME TO FINGER PRINT");
  lcd.setCursor(0, 1);
  lcd.write(" ACCESS  SYSTEM");
   Serial.println("ACCESS  SYSTEM");

   Serial.println("Waiting For Valid Finger");
}

void loop()                     // run over and over again
{


  digitalWrite(led, LOW);
  getFingerprintIDez();

  delay(50);

  //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
  case FINGERPRINT_OK:
        Serial.println("Image taken");
    break;
  case FINGERPRINT_NOFINGER:
       Serial.println("No finger detected");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
       Serial.println("Communication error");
    return p;
  case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
    return p;
  default:
      Serial.println("Unknown error");
    return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
  case FINGERPRINT_OK:
       Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
       Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
       Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
       Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
       Serial.println("Could not find fingerprint features");
    return p;
  default:
       Serial.println("Unknown error");
    return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
       Serial.println("Found a print match!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
       Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println("Did not find a match");
    return p;
  }
  else {
        Serial.println("Unknown error");
    return p;
  }

  //  // found a match!
  //  Serial.print("Found ID #");
  //  Serial.print(finger.fingerID);
  //  Serial.print(" with confidence of 1 ");
  //  Serial.println(finger.confidence);
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  //  // found a match!
  //  Serial.print("Found ID #");
  //  Serial.print(finger.fingerID);
  //  Serial.print(" with confidence of 2 ");
  //  Serial.println(finger.confidence);

  lcd.clear();


  if(finger.fingerID == 2) {
 /*   digitalWrite(led, HIGH);*/
    Serial.print("MEMBER ID :");
    Serial.println(finger.fingerID, DEC);
    lcd.setCursor(0, 0);
    lcd.write(" ACCESS GRANTED ");
    lcd.setCursor(0, 1);
    lcd.write("   ID :");
    lcd.print(finger.fingerID, DEC);

    second();
  }
  else if(finger.fingerID == 1 || finger.fingerID == 0){  
    Serial.print("ADMIN  ID :");
    Serial.println(finger.fingerID, DEC);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("ADMIN MODE ");
    lcd.print(finger.fingerID, DEC);
    lcd.setCursor(0, 1);
    lcd.write("SEQUENCE CODE :");
    delay(1000);

    KeyPad();

  }

  else{
 /*   digitalWrite(led, HIGH);*/
    Serial.print("MEMBER ID :");
    Serial.println(finger.fingerID, DEC);    
    lcd.setCursor(0, 0);
    lcd.write(" ACCESS GRANTED ");
   // lcd.print(finger.fingerID, DEC);
    lcd.setCursor(0, 1);
    lcd.write("   ID :");
    lcd.print(finger.fingerID, DEC);
     second();
  }
  
}
void Door(){
  digitalWrite(bell, HIGH);
  delay(200);
  digitalWrite(bell, LOW);
  delay(200);
  digitalWrite(bell, HIGH);
  delay(200);
  digitalWrite(bell, LOW);
  delay(5400);
  digitalWrite(led, LOW);
  delay(200);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("  FINGER PRINT");
  lcd.setCursor(0, 1);
  lcd.write(" ACCESS  SYSTEM");
}

void KeyPad(){
  digitalWrite(bell, HIGH);
  delay(200);
  digitalWrite(bell, LOW);
  char key = keypad.getKey();
  for (int i = 0;i < 4;i++){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(" ENTER SEQUENCE");
    lcd.setCursor(0, 1);
    lcd.write("CODE : ");
    pass = ".";
    while (pass != ""){
      key = keypad.getKey();
      if (key != NO_KEY){
        if(key == '#'){
          if(pass == ".1234"){
            pass = "";
            digitalWrite(bell, HIGH);
            delay(200);
            digitalWrite(bell, LOW);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.write("    WELCOME");
            lcd.setCursor(0, 1);
            lcd.write(" Administrator");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.write("A:ADD    C:BACK");
            lcd.setCursor(0, 1);
            lcd.write("B:Remove D:EXIT");
            int x =0;
            while(x == 0){
              key = keypad.getKey();
              if (key != NO_KEY){
                if(key == 'A'){
                  digitalWrite(bell, HIGH);
                  delay(200);
                  digitalWrite(bell, LOW);

                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.write("TYPE ID");
                  lcd.setCursor(0, 1);
                  lcd.write("ID : ");

                  uint8_t id = 0;
                  int a = 0;
                  while (a == 0) {
                    key = keypad.getKey();
                    if (key != NO_KEY){
                      if(key == '#'){
                        id = pass.toInt();
                        pass = "";
                        a=1;
                      }
                      else if(key == '*'){
                        pass = "";
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.write("TYPE ID");
                        lcd.setCursor(0, 1);
                        lcd.write("ID : ");
                      }
                      else if(key == 'D'){
                        pass = "";
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.write("  FINGER PRINT");
                        lcd.setCursor(0, 1);
                        lcd.write(" ACCESS  SYSTEM");
                        return;
                      }
                      else{
                        pass = pass + key;
                        lcd.write(key);
                        //                        Serial.println(pass);
                      }
                    }
                  }
                 Serial.print("Enrolling ID #");
                 Serial.println(id);
                  while (!  getFingerprintEnroll(id) );
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.write("A:ADD    C:BACK");
                  lcd.setCursor(0, 1);
                  lcd.write("B:Remove D:EXIT");
                }
                else if(key == 'B'){
                  digitalWrite(bell, HIGH);
                  delay(200);
                  digitalWrite(bell, LOW);
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.write("TYPE ID : ");
                  //Serial.println("Type in the ID # you want to save this finger as...");
                  uint8_t id = 0;
                  int a = 0;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.write("TYPE ID : ");
                  lcd.setCursor(0, 1);
                  lcd.write("ID : ");
                  while (a == 0) {
                    key = keypad.getKey();
                    if (key != NO_KEY){
                      if(key == '#'){
                        id = pass.toInt();
                        pass = "";
                        a=1;
                      }
                      else if(key == '*'){
                        pass = "";
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.write("TYPE ID : ");
                        lcd.setCursor(0, 1);
                        lcd.write("ID : ");
                      }
                      else if(key == 'D'){
                        pass = "";
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.write("  FINGER PRINT");
                        lcd.setCursor(0, 1);
                        lcd.write(" ACCESS  SYSTEM");
                        return;
                      }
                      else{
                        pass = pass + key;
                        lcd.write(key);
                        //
                      }
                    }
                  }

                  deleteFingerprint(id);
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.write("A:ADD    C:BACK");
                  lcd.setCursor(0, 1);
                  lcd.write("B:Remove D:EXIT");
                }
                else if(key == 'C'){
                  digitalWrite(bell, HIGH);
                  delay(200);
                  digitalWrite(bell, LOW);
                  pass = "";
                  i = 0;
                  x = 1;
                }
                else if(key == 'D'){
                  digitalWrite(bell, HIGH);
                  delay(200);
                  digitalWrite(bell, LOW);
                  delay(200);
                  digitalWrite(bell, HIGH);
                  delay(200);
                  digitalWrite(bell, LOW);
                  pass = "";
                  i = 3;
                  x = 1;

                }


              }
            }
          }
          else{
            pass = "";
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.write("WRONG PASSWORD");
            //            Serial.println("Wrong Password");
            digitalWrite(bell, HIGH);
            delay(1000);
            digitalWrite(bell, LOW);
          }
        }
        else if(key == 'D'){
          pass = "";
          i = 4;
          digitalWrite(bell, HIGH);
          delay(200);
          digitalWrite(bell, LOW);
          delay(200);
          digitalWrite(bell, HIGH);
          delay(200);
          digitalWrite(bell, LOW);
        }
        else if(key == '*'){
          digitalWrite(bell, HIGH);
          delay(200);
          digitalWrite(bell, LOW);

          pass = "";
        }
        else{
          pass = pass + key;
          lcd.setCursor(7, 1);
          for(int x = 2 ; x <= pass.length() ; x++){
            lcd.print("x");
          }
          if(pass.length() > 8){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.write("WRONG PASSWORD");
            pass = "";
            digitalWrite(bell, HIGH);
            delay(1000);
            digitalWrite(bell, LOW);
          }
          //          Serial.println(pass);
        }
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("  FINGER PRINT");
  lcd.setCursor(0, 1);
  lcd.write(" ACCESS  SYSTEM");
}

void password(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("    FP ERROR");
  lcd.setCursor(0, 1);
  lcd.write("MANUAL MODE ON");
  while(true){
    char key = keypad.getKey();
    if (key != NO_KEY){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write("MANUAL MODE ON");
      lcd.setCursor(0, 1);
      lcd.write("CODE : ");
      if(key == '#'){
        if(pass == "12345"){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.write("WELCOME ADMIN");
          //          Serial.println("WELCOME ADMIN");
          digitalWrite(led, HIGH);
          digitalWrite(bell, HIGH);
          delay(200);
          digitalWrite(bell, LOW);
          delay(200);
          digitalWrite(bell, HIGH);
          delay(200);
          digitalWrite(bell, LOW);
          delay(4400);
          digitalWrite(led, LOW);
          pass = "";
          password();
        }
        else{
          pass = "";
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.write("WRONG PASSWORD");
          //          Serial.println("Wrong Password");
          digitalWrite(bell, HIGH);
          delay(1000);
          digitalWrite(bell, LOW);
          password();
        }
      }
      else if(key == 'D'){
        pass = "";
        setup();
      }
      else if(key == '*'){
        pass = "";
      }
      else{
        pass = pass + key;
        lcd.setCursor(7, 1);
        for(int x = 1 ; x <= pass.length() ; x++){
          lcd.write("*");
        }
        if(pass.length() > 8){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.write("WRONG PASSWORD");
          pass = "";
          digitalWrite(bell, HIGH);
          delay(1000);
          digitalWrite(bell, LOW);
        }
      }
    }
  }
}

uint8_t getFingerprintEnroll(uint8_t id) {
  uint8_t p = -1;
  Serial.println("Waiting for valid finger to enroll");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("Waiting for");
  lcd.setCursor(0, 1);
  lcd.write("finger to enroll");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;

    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
   Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.write("REMOVE FINGER");
  lcd.setCursor(4, 1);
  lcd.write("FROM FP");
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.write("PLACE SAME");
  lcd.setCursor(2, 1);
  lcd.write("FINGER AGAIN");

  //  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //      Serial.println("Imaging error");
      break;
    default:
      //      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }


  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //    Serial.println("Prints matched!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("PRINTS MATCHED!");
    delay(1000);
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //    Serial.println("Fingerprints did not match");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("DIDN'T MATCHED");
    delay(1000);
    return p;
  }
  else {
    //    Serial.println("Unknown error");
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //    Serial.println("Stored!");
    lcd.setCursor(2, 1);
    lcd.write("DATA STORED!");
    delay(1000);
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
    //    Serial.println("Could not store in that location");
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR) {
    //    Serial.println("Error writing to flash");
    return p;
  }
  else {
    //    Serial.println("Unknown error");
    return p;
  }
}


uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("ID : ");
    lcd.print(id, DEC);
    lcd.setCursor(0, 1);
    lcd.write("FP DELETED");
    delay(2000);
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
//    Serial.println("Could not delete in that location");
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR) {
//    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.print("Unknown error: 0x");
    Serial.println(p, HEX);
    return p;
  }
}
void Play_Pirates()
{ 
 
    tone(12,500);
		delay(3000);
		noTone(12);}






void second(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("SECOND VERIFICATION");
					lcd.setCursor(0, 1);
  lcd.write("CODE: ");
  
  while(true){
    char key = keypad.getKey();
    if (key != NO_KEY){
     lcd.clear();
												 lcd.setCursor(0, 0);
  lcd.write("SECOND VERIFICATION");
												lcd.setCursor(0, 1);
  lcd.write("CODE: ");
											
      if(key == '#'){
        if(pass == "123456"){
          lcd.clear();
																				lcd.setCursor(0, 0);
          lcd.write("ACCESS GRANTED ");
          lcd.setCursor(3, 1);
          lcd.write("WELCOME ");
          //          Serial.println("WELCOME ADMIN");
          digitalWrite(led, HIGH);
									
	tone(12,tune);
		delay(3000);
		noTone(12);

		
		  
  
																				
																		/*		return getFingerprintIDez();*/
																				return setup();
         
          
        }
        else{
          pass = "";
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.write("WRONG PASSWORD");
          //          Serial.println("Wrong Password");
     
          second();
        }
      }
      else if(key == 'D'){
        pass = "";
        return setup();
      }
      else if(key == '*'){
        pass = "";
													
      }
      else{
        pass = pass + key;
        lcd.setCursor(5, 1);
        for(int x = 1 ; x <= pass.length() ; x++){
          lcd.write("*");
        }
        if(pass.length() > 8){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.write("WRONG PASSWORD");
          pass = "";
          
																				second();
        }
      }
												
    }
  }
				
}
void tune(){292;}