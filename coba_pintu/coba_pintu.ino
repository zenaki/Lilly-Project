#include <Servo.h>

Servo Pintu_1;
Servo Pintu_2;

int pos = 0;

void setup() {
  Serial.begin(115200);

  Pintu_1.attach(6);
  Pintu_2.attach(9);
}

void loop() {
  Pintu_1.write(105);
  Pintu_2.write(100);
  delay(2000);
  Pintu_1.write(20);
  Pintu_2.write(20);
  delay(2000);
}
