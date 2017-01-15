//** LCD I2C **//
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
//** LCD I2C **//

//** Serve Pintu **//
#include <Servo.h>

Servo Pintu_1;
Servo Pintu_2;
//** Serve Pintu **//

//** FingerPrint **//
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

uint8_t getFingerprintEnroll(int id);

// pin #10 is IN from sensor (GREEN wire)
// pin #11 is OUT from arduino  (WHITE wire)
//SoftwareSerial mySerial(2, 3);
SoftwareSerial mySerial(10, 11);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//** FingerPrint **//

//** RFID **//
#include <SPI.h>
#include <MFRC522.h>

//#define SS_PIN 10
//#define RST_PIN 9
#define SS_PIN 53
#define RST_PIN 5
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[3];
//** RFID **//

int Buz = 4;
int Disc_1 = 7;
int Disc_2 = 8;
int Limit_1 = 2;
int Limit_2 = 3;

int pb_pintu_1 = 15;
int pb_pintu_2 = 16;
int pb_pagar = 14;

bool buka_pagar = false, tutup_pagar = true;
bool buka_pintu_1 = false, tutup_pintu_1 = true;
bool buka_pintu_2 = false, tutup_pintu_2 = true;

char DataSerial1[10], DataSerial2[10];
bool fng = false, erl = false, dapetJari = false, simpanJari = false;
char DataSerial[50], *temp;

bool debug = false;

void setup() {
  Serial.begin(57600);
//  Serial.begin(57600);

  lcd.begin();
  lcd.backlight();
  
  Pintu_1.attach(6);
  Pintu_2.attach(9);

  pinMode(Limit_1, INPUT_PULLUP);
  pinMode(Limit_2, INPUT_PULLUP);
  pinMode(Disc_1, OUTPUT);
  pinMode(Disc_2, OUTPUT);

  pinMode(pb_pintu_1, INPUT_PULLUP);
  pinMode(pb_pintu_2, INPUT_PULLUP);
  pinMode(pb_pagar, INPUT_PULLUP);
  
  digitalWrite(Disc_1, LOW);
  digitalWrite(Disc_2, LOW);
  
  finger.begin(57600);

  memset(DataSerial, '\0', 50);
  
  if (finger.verifyPassword()) {
    if (debug) {
      Serial.println("Found fingerprint sensor!");
      Serial.println("Kirim : enroll;ID# / finger;0#");
    } else {
      Serial.println("#FP::Finger Print Found*");
    }
    Display_LCD_str("Welcome", 0, 0);
  } else {
    if (debug) {
      Serial.println("Did not find fingerprint sensor :(");
//      while (1);
    } else {
      Serial.println("#FP::Finger Print NOT Found*");
    }
    Display_LCD_str("Error", 0, 0);
  }

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

//  Serial.println(F("This code scan the MIFARE Classsic NUID."));
//  Serial.print(F("Using the following key:"));
//  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  fng = true; erl = false;

  tutupPintu1();
  tutupPintu2();
}

void loop() {
  if (erl && atoi(DataSerial2) != 0) {
    int id = atoi(DataSerial2);
    if (debug) {
      Serial.println("Enroll Fingerprint");
      Serial.print("Enrolling ID #");
      Serial.println(id);
    } else {
//      Serial.print("#FP::Pendaftaran Jari Dengan ID : ");
//      Serial.print(id);
//      Serial.println("*");
    }
    Display_LCD_str("Enroll ID: ", 0, 0);
    Display_LCD_int(id, 0, 11);
    while (!getFingerprintEnroll(id));
    if (simpanJari) {
      if (debug) {
        Serial.println();
        Serial.print("stored;");
        Serial.print(id, DEC);
        Serial.println("#");
      } else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Berhasil Tersimpan*");
      }
      Display_LCD_str("Stored ID: ", 0, 0);
      Display_LCD_int(id, 0, 11);
    } else {
      if (debug) {
        Serial.println();
        Serial.print("FAIL store with this ID ");
        Serial.print(id, DEC);
        Serial.println("#");
      } else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }
      Display_LCD_str("Fail ID: ", 0, 0);
      Display_LCD_int(id, 0, 9);
    }
    fng = true; erl = false;
  } else if (fng) {
    if (debug) {
      Serial.println("Read Fingerprint");
    }
    Display_LCD_str("Waiting Finger..", 0, 0);
    Display_LCD_str("                ", 1, 0);
    while(1) {
      proses_pagar();
      proses_pintu();
      
      readSerial();

      proses_pagar();
      proses_pintu();
      if (fng) {
        RFID_read();
        getFingerprintIDez();
        delay(50);            //don't need to run this at full speed.
        RFID_read();
        if (dapetJari && finger.confidence >= 80) {
          if (debug) {
            Serial.print("jari;");
            Serial.print(finger.fingerID, DEC);
            Serial.println("#");
            dapetJari = false;
          } else {
            Serial.print("#FP::GET ");
            Serial.print(finger.fingerID, DEC);
            Serial.println("*");
            dapetJari = false;
          }
          Display_LCD_str("                ", 1, 0);
          Display_LCD_str("GET ID: ", 1, 0);
          Display_LCD_int(finger.fingerID, 1, 8);
          
          if (finger.fingerID == 100 && !buka_pintu_1) bukaPintu1();
          if (finger.fingerID == 101 && !buka_pintu_2) bukaPintu2();

          proses_pagar();
          proses_pintu();
//          checkFinger();
          
        }
      } else {
//        Serial.println("#FP::Keluar Dari Pemeriksaan Jari ...*");
        break;
      }
    }
  }
  RFID_read();
}

uint8_t getFingerprintEnroll(int id) {
  int tOut = 200, c_tOut = 0;
  simpanJari = false;

  int p = -1;
  if (debug) {
    Serial.println("Waiting for valid finger to enroll");
  } else {
    Serial.println("#FP::Daftar::Menunggu Jari ...*");
  }
  Display_LCD_str("Menunggu Jari ...", 0, 0);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      if (debug) Serial.println("Image taken");
      Display_LCD_str("Dapat Jari ...", 0, 0);
      break;
    case FINGERPRINT_NOFINGER:
      if (debug) Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      if (debug) Serial.println("Communication error");
      Display_LCD_str("ERROR ...", 0, 0);
      break;
    case FINGERPRINT_IMAGEFAIL:
      if (debug) Serial.println("Imaging error");
      Display_LCD_str("Error ...", 0, 0);
      break;
    default:
      if (debug) Serial.println("Unknown error");
      Display_LCD_str("Error ...", 0, 0);
      break;
    }
    c_tOut++;
    if (c_tOut > tOut) return true;
    RFID_read();
  }
  c_tOut = 0;

//  OK success!
//  if (!debug) Serial.println("#FP::Daftar::Pertahankan Jari Di Sensor ...*");
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      if (debug) Serial.println("Image converted");
      Display_LCD_str("Pertahankan Jari ...", 0, 0);
      break;
    case FINGERPRINT_IMAGEMESS:
      if (debug) {Serial.println("Image too messy");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      if (debug) {Serial.println("Communication error");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      if (debug) {Serial.println("Could not find fingerprint features");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      if (debug) {Serial.println("Could not find fingerprint features");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    default:
      if (debug) {Serial.println("Unknown error");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
  }

  if (debug) {
    Serial.println("Remove finger");
  } else {
    Serial.println("#FP::Daftar::Jauhkan Jari*");
  }
  Display_LCD_str("Jauhkan Jari ..", 0, 0);
  delay(2000);

  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  if (debug) {
    Serial.println("Place same finger again");
  } else {
    Serial.println("#FP::Daftar::Tempelkan Jari Sebelumnya*");
  }
  Display_LCD_str("Tempelkan Jari ..", 0, 0);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        if (debug) {Serial.println(); Serial.println("Image taken");}
        Display_LCD_str("Pertahankan Jari ...", 0, 0);
        break;
      case FINGERPRINT_NOFINGER:
        if (debug) Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        if (debug) {Serial.println("Communication error");} /*else {
          Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
        }*/
        simpanJari = false;
        Display_LCD_str("Error ...", 0, 0);
        return p;
      case FINGERPRINT_IMAGEFAIL:
        if (debug) {Serial.println("Imaging error");} /*else {
          Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
        }*/
        simpanJari = false;
        Display_LCD_str("Error ...", 0, 0);
        return p;
      default:
        if (debug) {Serial.println("Unknown error");} /*else {
          Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
        }*/
        simpanJari = false;
        Display_LCD_str("Error ...", 0, 0);
        return p;
    }
    c_tOut++;
    if (c_tOut > tOut) return true;
    RFID_read();
  }
  c_tOut = 0;

//  OK success!
//  if (!debug) Serial.println("#FP::Daftar::Pertahankan Jari Di Sensor ...*");
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      if (debug) Serial.println("Image converted");
      Display_LCD_str("Pertahankan Jari ...", 0, 0);
      break;
    case FINGERPRINT_IMAGEMESS:
      if (debug) {Serial.println("Image too messy");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      if (debug) {Serial.println("Communication error");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      if (debug) {Serial.println("Could not find fingerprint features");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      if (debug) {Serial.println("Could not find fingerprint features");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
    default:
      if (debug) {Serial.println("Unknown error");} /*else {
        Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
      }*/
      simpanJari = false;
      Display_LCD_str("Error ...", 0, 0);
      return p;
  }

//  OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    if (debug) Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    if (debug) {Serial.println("Communication error");} /*else {
      Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
    }*/
    simpanJari = false;
    Display_LCD_str("Error ...", 0, 0);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    if (debug) {Serial.println("Fingerprints did not match");} /*else {
      Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
    }*/
    simpanJari = false;
    Display_LCD_str("Error ...", 0, 0);
    return p;
  } else {
    if (debug) {Serial.println("Unknown error");} /*else {
      Serial.print("#FP::ID "); Serial.print(id); Serial.println(" Gagal Tersimpan*");
    }*/
    simpanJari = false;
    Display_LCD_str("Error ...", 0, 0);
    return p;
  }   
  
  if (debug) {
    Serial.print("ID #"); Serial.print(id);
  } /*else {
    Serial.print("#FP::ID "); Serial.print(id);
  }*/
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    if (debug) {
      Serial.println(" Stored!");
    } /*else {
      Serial.println(" Berhasil Tersimpan*");
    }*/
    simpanJari = true;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    if (debug) {
      Serial.println("Communication error");
    } /*else {
      Serial.println(" Gagal Tersimpan*");
    }*/
    simpanJari = false;
    Display_LCD_str("Error ...", 0, 0);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    if (debug) {
      Serial.println("Could not store in that location");
    } /*else {
      Serial.println(" Gagal Tersimpan*");
    }*/
    simpanJari = false;
    Display_LCD_str("Error ...", 0, 0);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    if (debug) {
      Serial.println("Error writing to flash");
    } /*else {
      Serial.println(" Gagal Tersimpan*");
    }*/
    simpanJari = false;
    Display_LCD_str("Error ...", 0, 0);
    return p;
  } else {
    if (debug) {
      Serial.println("Unknown error");
    } /*else {
      Serial.println(" Gagal Tersimpan*");
    }*/
    simpanJari = false;
    Display_LCD_str("Error ...", 0, 0);
    return p;
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
//  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
//  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
//  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  dapetJari = true;
  return finger.fingerID; 
}

void RFID_read() {
  proses_pagar();
  proses_pintu();
  
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

//  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
//    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  if (debug) {
    Serial.print("rfid;");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println("#");
  } else {
    Serial.print("#RFID::GET ");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println("*");
  }
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  bukaPagar();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  int j = 9;
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
//    lcd.setCursor(j++, 1);
//    lcd.print(buffer[i] < 0x10 ? " 0" : " ");
//    lcd.setCursor(j++, 1);
//    lcd.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void readSerial() {
  while (Serial.available()) {
    char c[2]; c[1] = '\0'; c[0] = Serial.read();
    if (c[0] == 0x0d || c[0] == 0x0a) continue;
      
    strcat(DataSerial, c);
      
    if (strchr(DataSerial, '#') != NULL) {
      temp = strtok (DataSerial,";#");
      int cnt = 0;
      memset(DataSerial1, '\0', 10);
      memset(DataSerial2, '\0', 10);
      while (temp != NULL) {
//        Serial.println(temp);
        if (cnt == 0) strcpy(DataSerial1, temp);
        if (cnt == 1) strcpy(DataSerial2, temp);
        temp = strtok (NULL, ";#");
        cnt++;
      }

      if (strcmp(DataSerial1, "enroll") == 0 && strcmp(DataSerial2, "0") != 0) {erl = true; fng = false;} 
      else if (strcmp(DataSerial1, "finger") == 0 && strcmp(DataSerial2, "0") == 0) {erl = false; fng = true;} 
      memset(DataSerial, '\0', 50);

      if (debug) {
        Serial.print("DataSerial1 = "); Serial.print(DataSerial1); Serial.println(",");
        Serial.print("DataSerial2 = "); Serial.print(DataSerial2); Serial.println(",");
      }
    }
  }
}

void checkFinger() {
  bool finish = false;
  while (1) {
    while (Serial.available()) {
      char c[2]; c[1] = '\0'; c[0] = Serial.read();
      if (c[0] == 0x0d || c[0] == 0x0a) continue;
      
      strcat(DataSerial, c);
      
      if (strchr(DataSerial, '#') != NULL) {
        temp = strtok (DataSerial,";#");
        int cnt = 0;
        memset(DataSerial1, '\0', 10);
        memset(DataSerial2, '\0', 10);
        while (temp != NULL) {
//          Serial.println(temp);
          if (cnt == 0) strcpy(DataSerial1, temp);
          if (cnt == 1) strcpy(DataSerial2, temp);
          temp = strtok (NULL, ";#");
          cnt++;
        }
        
        if (strcmp(DataSerial1, "true") == 0) {
          Display_LCD_str("Buka Kamar : ", 0, 0);
          Display_LCD_str(DataSerial2, 0, 13);
          finish = true;
        } else if (strcmp(DataSerial1, "false") == 0) {
          Display_LCD_str("Jari Belum Terdaftar ..", 0, 0);
          finish = true;
        } 
        memset(DataSerial, '\0', 50);
        
        if (debug) {
          Serial.print("DataSerial1 = "); Serial.print(DataSerial1); Serial.println(",");
          Serial.print("DataSerial2 = "); Serial.print(DataSerial2); Serial.println(",");
        }
      }
    }
    if (finish) {
      break;
    }
  }
}

void Display_LCD_str(const char vC[], int vCursor_Row, int vCursor_Col) {
  lcd.setCursor(vCursor_Col, vCursor_Row);
  lcd.print(vC);
}

void Display_LCD_int(int vC, int vCursor_Row, int vCursor_Col) {
  lcd.setCursor(vCursor_Col, vCursor_Row);
  lcd.print(vC);
}

void bukaPagar() {
  digitalWrite(Disc_1, HIGH);
  digitalWrite(Disc_2, LOW);
}

void bukaPintu1() {
  Pintu_1.write(20);
  buka_pintu_1 = true; tutup_pintu_1 = false;
}

void bukaPintu2() {
  Pintu_2.write(20);
  buka_pintu_2 = true; tutup_pintu_2 = false;
}

void tutupPagar() {
  digitalWrite(Disc_1, LOW);
  digitalWrite(Disc_2, HIGH);
}

void tutupPintu1() {
  Pintu_1.write(105);
  buka_pintu_1 = false; tutup_pintu_1 = true;
}

void tutupPintu2() {
  Pintu_2.write(100);
  buka_pintu_2 = false; tutup_pintu_2 = true;
}

void proses_pagar() {
  int pb_pgr = digitalRead(pb_pagar);
//  if (debug) {
//    Serial.print("pb_pagar = "); Serial.println(pb_pgr);
//  }
  if (pb_pgr == 0 && !tutup_pagar) {
    tutupPagar();
  } else {
    int ls_1 = digitalRead(Limit_1);
    if (!ls_1) {
      digitalWrite(Disc_1, LOW);
      digitalWrite(Disc_2, LOW);
      buka_pagar = false; tutup_pagar = true;
    }
    
    int ls_2 = digitalRead(Limit_2);
    if (!ls_2) {
      digitalWrite(Disc_1, LOW);
      digitalWrite(Disc_2, LOW);
      buka_pagar = true; tutup_pagar = false;
    }
  }
}

void proses_pintu() {
  int pb_pnt_1 = digitalRead(pb_pintu_1);
//  if (debug) {
//    Serial.print("pb_pintu_1 = "); Serial.println(pb_pnt_1);
//  }
  if (pb_pnt_1 == 0 && buka_pintu_1) {
//    if (buka_pintu_1) {
      tutupPintu1();
//    } else if (tutup_pintu_1) {
//      bukaPintu1();
//    }
  }

  int pb_pnt_2 = digitalRead(pb_pintu_2);
//  if (debug) {
//    Serial.print("pb_pintu_2 = "); Serial.println(pb_pnt_2);
//  }
  if (pb_pnt_2 == 0 && buka_pintu_2) {
//    if (buka_pintu_2) {
      tutupPintu2();
//    } else if (tutup_pintu_2) {
//      bukaPintu2();
//    }
  }
}

