#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte heart[2][8] = {
  { B00000, B01010, B11111, B11111, B01110, B00100, B00000, B00000 },
  { B11011, B11111, B11111, B11111, B11111, B01110, B00100, B00000 }
};

unsigned long phase, last;
long pot = 512L * 8;
int bpm, shown, beat = -1;

void setup() {
  lcd.init();
  lcd.backlight();
  last = millis();
}

void loop() {
  unsigned long now = millis();
  pot += analogRead(A0) - pot / 8;
  bpm = map(pot / 8, 0, 1023, 40, 180);

  phase = (phase + (now - last) * bpm) % 60000;
  last = now;
  int t = phase / bpm;
  int b = t < 140 || (t > 200 && t < 280);

  if (b != beat) {
    beat = b;
    lcd.createChar(0, heart[b]);
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
  }

  if (bpm != shown) {
    shown = bpm;
    char s[8];
    sprintf(s, "%3d BPM", bpm);
    lcd.setCursor(9, 0);
    lcd.print(s);
  }
}
