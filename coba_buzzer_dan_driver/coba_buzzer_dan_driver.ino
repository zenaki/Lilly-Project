
int Buz = 4;
int Disc_1 = 7;
int Disc_2 = 8;
int Limit_1 = 2;
int Limit_2 = 3;

bool buka = false, tutup = false;

void setup() {
  Serial.begin(9600);
  pinMode(Buz, OUTPUT);
  pinMode(Limit_1, INPUT_PULLUP);
  pinMode(Limit_2, INPUT_PULLUP);
  pinMode(Disc_1, OUTPUT);
  pinMode(Disc_2, OUTPUT);

  digitalWrite(Disc_1, LOW);
  digitalWrite(Disc_2, LOW);
}

void loop() {
  int ls_1 = digitalRead(Limit_1);
  int ls_2 = digitalRead(Limit_2);
  
  if (!ls_1) {
    buz_buka();
    buka = true; tutup = false;
  }

  if (!ls_2) {
    buz_tutup();
    buka = false; tutup = true;
  }

  Serial.print("Limit_1 : ");
  Serial.print(ls_1);
  Serial.print(", Limit_2 : ");
  Serial.print(ls_2);
  Serial.print(", Buka : ");
  Serial.print(buka);
  Serial.print(", Tutup: ");
  Serial.print(tutup);
  Serial.println();

  if (buka) {
//    digitalWrite(Disc_1, LOW);
//    digitalWrite(Disc_2, LOW);
//    delay(5000);
    digitalWrite(Disc_1, HIGH);
    digitalWrite(Disc_2, LOW);
  }

  if (tutup) {
//    digitalWrite(Disc_1, LOW);
//    digitalWrite(Disc_2, LOW);
//    delay(5000);
    digitalWrite(Disc_1, LOW);
    digitalWrite(Disc_2, HIGH);
  }
}

void buz_buka() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(Buz, HIGH);
    delay(100);
    digitalWrite(Buz, LOW);
    delay(100);
  }
}

void buz_tutup() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(Buz, HIGH);
    delay(100);
    digitalWrite(Buz, LOW);
    delay(100);
  }
}

