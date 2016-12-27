void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

}

void loop() {
  int Mag_1 = digitalRead(3);
  int Mag_2 = digitalRead(2);

  Serial.print("Mag_1 : ");
  Serial.print(Mag_1);
  Serial.print(", Mag_2 : ");
  Serial.println(Mag_2);

  delay(10);

}
