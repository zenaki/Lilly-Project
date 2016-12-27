#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

int value;

void setup() {
  lcd.begin();
  lcd.backlight();

  pinMode(A0, INPUT);
  
  lcd.setCursor(0,0);
  lcd.print("Value : ");
  lcd.setCursor(0,1);
  lcd.print("Value : ");
}

void loop() {
  int data = analogRead(A0);

  lcd.setCursor(8,0);
  lcd.print("    ");
  lcd.setCursor(8,1);
  lcd.print("    ");
  
  lcd.setCursor(8,0);
  lcd.print(data);
  lcd.setCursor(8,1);
  lcd.print(data);

  delay(10);
}

//void LCD_Print(int len, int col) {
//  for (int i = 0; i < 16; i++) {
//    if (i < len) {
//      lcd.setCursor(i, col);
//      lcd.print(DataLCD[i]);
//    } else {
//      lcd.setCursor(i, col);
//      lcd.print(" ");
//    }
//  }
//}
