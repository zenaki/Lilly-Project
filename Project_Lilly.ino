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

char DataSerial1[10], DataSerial2[10];
bool fng = false, erl = false, dapetJari = false, simpanJari = false;
char DataSerial[50], *temp;

bool debug = false;

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
  finger.begin(57600);

  memset(DataSerial, '\0', 50);
  
  if (finger.verifyPassword()) {
    if (debug) {
      Serial.println("Found fingerprint sensor!");
      Serial.println("Kirim : enroll;ID# / finger;0#");
    } else {
      Serial.println("FP::Finger Print Found*");
    }
  } else {
    if (debug) {
      Serial.println("Did not find fingerprint sensor :(");
//      while (1);
    } else {
      Serial.println("FP::Finger Print NOT Found*");
    }
  }

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

//  Serial.println(F("This code scan the MIFARE Classsic NUID."));
//  Serial.print(F("Using the following key:"));
//  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop() {
  fng = false; erl = false;
  while (Serial.available()) {
    char c[2]; c[1] = '\0'; c[0] = Serial.read();
    if (c[0] == 0x0d || c[0] == 0x0a) continue;
      
    strcat(DataSerial, c);
      
    if (strchr(DataSerial, '#') != NULL) {
      temp = strtok (DataSerial,";#");
      int cnt = 0;
      memset(DataSerial1, '\0', 10);
      memset(DataSerial2, '\0', 10);
      while (temp != NULL)
      {
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
      } else {
        if (erl) {
          Serial.println("FP::Pendaftaran Jari*");
        } else if (fng) {
          Serial.println("FP::Menunggu Jari Ditempelkan*");
        }
      }
    }
  }
  
  if (erl && atoi(DataSerial2) != 0) {
    int id = atoi(DataSerial2);
    if (debug) {
      Serial.println("Enroll Fingerprint");
      Serial.print("Enrolling ID #");
      Serial.println(id);
    } else {
      Serial.print("FP::Pendaftaran Jari Dengan ID : ");
      Serial.print(id);
      Serial.println("*");
    }
    while (!getFingerprintEnroll(id));
    if (simpanJari) {
      if (debug) {
        Serial.print("stored;");
        Serial.print(id, DEC);
        Serial.println("#");
      }
    } else {
      if (debug) {
        Serial.print("FAIL store with this ID ");
        Serial.print(id, DEC);
        Serial.println("#");
      }
    }
  } else if (fng && atoi(DataSerial2) == 0) {
    if (debug) {
      Serial.println("Read Fingerprint");
    }
    while(1) {
      while (Serial.available()) {
        char c[2]; c[1] = '\0'; c[0] = Serial.read();
        if (c[0] == 0x0d || c[0] == 0x0a) continue;
      
        strcat(DataSerial, c);
      
        if (strchr(DataSerial, '#') != NULL) {
          temp = strtok (DataSerial,";#");
          int cnt = 0;
          memset(DataSerial1, '\0', 10);
          memset(DataSerial2, '\0', 10);
          while (temp != NULL)
          {
//            Serial.println(temp);
            if (cnt == 0) strcpy(DataSerial1, temp);
            if (cnt == 1) strcpy(DataSerial2, temp);
            temp = strtok (NULL, ";#");
            cnt++;
          }

          if (strcmp(DataSerial1, "finger") == 0) {erl = false; fng = false;} 
          memset(DataSerial, '\0', 50);

          if (debug) {
            Serial.print("DataSerial1 = "); Serial.print(DataSerial1); Serial.println(",");
            Serial.print("DataSerial2 = "); Serial.print(DataSerial2); Serial.println(",");
          }
        }
      }
      
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
            Serial.print("FP::GET ");
            Serial.print(finger.fingerID, DEC);
            Serial.println("#*");
            dapetJari = false;
          }
        }
      } else {
        Serial.println("FP::Keluar Dari Pemeriksaan Jari ...*");
        break;
      }
    }
  }
  RFID_read();
}

uint8_t getFingerprintEnroll(int id) {
  int p = -1;
  if (debug) {
    Serial.print("Waiting for valid finger to enroll");
  } else {
    Serial.println("FP::Menunggu Jari ...*");
  }
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      if (debug) Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      if (debug) Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      if (debug) Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      if (debug) Serial.println("Imaging error");
      break;
    default:
      if (debug) Serial.println("Unknown error");
      break;
    }
    RFID_read();
  }

  // OK success!
//  if (!debug) Serial.println("FP::Pertahankan Jari Di Sensor ...*");
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      if (debug) Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      if (debug) Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      if (debug) Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      if (debug) Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      if (debug) Serial.println("Could not find fingerprint features");
      return p;
    default:
      if (debug) Serial.println("Unknown error");
      return p;
  }
  if (debug) {
    Serial.println("Remove finger");
  } else {
    Serial.println("FP::Jauhkan Jari Dari Sensor ...*");
  }
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
    RFID_read();
  }

  p = -1;
  if (debug) {
    Serial.print("Place same finger again");
  } else {
    Serial.println("FP::Tempelkan Jari Sebelumnya ...*");
  }
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      if (debug) Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      if (debug) Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      if (debug) Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      if (debug) Serial.println("Imaging error");
      break;
    default:
      if (debug) Serial.println("Unknown error");
      break;
    }
    RFID_read();
  }

  // OK success!
//  if (!debug) Serial.println("FP::Pertahankan Jari Di Sensor ...*");
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      if (debug) Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      if (debug) Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      if (debug) Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      if (debug) Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      if (debug) Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    if (debug) Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    if (debug) Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    if (debug) Serial.println("Fingerprints did not match");
    return p;
  } else {
    if (debug) Serial.println("Unknown error");
    return p;
  }   

  if (debug) {
    Serial.print("ID #"); Serial.println(id);
  } else{
    Serial.print("FP::ID #"); Serial.print(id);
  }
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    if (debug) {
      Serial.println("Stored!");
    } else {
      Serial.println(" Berhasil Tersimpan*");
    }
    simpanJari = true;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    if (debug) {
      Serial.println("Communication error");
    } else {
      Serial.println(" Gagal Tersimpan*");
    }
    simpanJari = false;
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    if (debug) {
      Serial.println("Could not store in that location");
    } else {
      Serial.println(" Gagal Tersimpan*");
    }
    simpanJari = false;
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    if (debug) {
      Serial.println("Error writing to flash");
    } else {
      Serial.println(" Gagal Tersimpan*");
    }
    simpanJari = false;
    return p;
  } else {
    if (debug) {
      Serial.println("Unknown error");
    } else {
      Serial.println(" Gagal Tersimpan*");
    }
    simpanJari = false;
    return p;
  }
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
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

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
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

//  Serial1.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//  Serial1.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
//    Serial1.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
    
  if (debug) {
    Serial1.print("rfid;");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial1.println("#");
  } else {
    Serial1.print("RFID::GET ");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial1.println("#*");
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial1.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial1.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial1.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial1.print(buffer[i], DEC);
  }
}
