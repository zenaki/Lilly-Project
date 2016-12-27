
int Buz = 4;
int Disc_1 = 7;
int Disc_2 = 8;

void setup() {
  pinMode(Buz, OUTPUT);
  pinMode(Disc_1, OUTPUT);
  pinMode(Disc_2, OUTPUT);

}

void loop() {
  digitalWrite(Buz, HIGH);
  digitalWrite(Disc_1, HIGH); // Maju
  digitalWrite(Disc_2, LOW);
  delay(2000);

  digitalWrite(Buz, LOW);
  digitalWrite(Disc_1, LOW); // Mundur
  digitalWrite(Disc_2, HIGH);
  delay(2000);
}
