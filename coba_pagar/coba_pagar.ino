void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

}

void loop() {
  int Limit_1 = digitalRead(3);
  int Limit_2 = digitalRead(2);

  Serial.print("Limit_1 : ");
  Serial.print(Limit_1);
  Serial.print(", Limit_2 : ");
  Serial.println(Limit_2);

  delay(10);

}
