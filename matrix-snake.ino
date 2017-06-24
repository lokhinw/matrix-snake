#include <LedControl.h>

unsigned long delayTime = 100;

int level;
int joyX = 0;
int joyY = 1;
int joyVal;
int DIN = 12;
int CLK = 11;
int CS = 10;

LedControl lc = LedControl(DIN, CLK, CS, 1);

byte s[8] = {0x00, 0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C};
byte n[8] = {0x00, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x63, 0x63};
byte a[8] = {0x00, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66};
byte k[8] = {0x00, 0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66};
byte e[8] = {0x00, 0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E};
byte one[8] = {0x00, 0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7E};
byte two[8] = {0x00, 0x3C, 0x66, 0x06, 0x0C, 0x30, 0x60, 0x7E};
byte three[8] = {0x00, 0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C};
byte fill[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setup() {
//Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  printByte(s);
  delay(1000);
  printByte(n);
  delay(1000);
  printByte(a);
  delay(1000);
  printByte(k);
  delay(1000);
  printByte(e);
  delay(1000);
  printByte(fill);
  delay(1000);
  level = 1;
  printByte(one);
}

void loop() {
  joyVal = analogRead(joyX);
  if (joyVal <= 341) {
    if (!(level >= 3)) {
      level +=1;
    }
  } else if (joyVal >= 682) {
     if (!(level <= 1)) {
      level -=1;
    }
  }
  if (level == 1) {
    printByte(one);  
  } else if (level == 2) {
    printByte(two);
  } else if (level == 3) {
    printByte(three);
  }
  delay(200);
}

void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
