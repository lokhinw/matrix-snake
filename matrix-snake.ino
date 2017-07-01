#include <LedControl.h>

unsigned long delayTime = 100;

int level;
int snakeSpeed;
int snakeDirection; // 1 = UP; 2 = RIGHT; 3 = DOWN; 4 = LEFT
int snakeLength;
int foodX, foodY;
int joyX = A0;
int joyY = A1;
int joyBtn = 2;
int joyValX, joyValY;
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
byte grid[8][8];

void setup()
{
  Serial.begin(9600);
  pinMode(joyBtn, INPUT_PULLUP);
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

void loop() 
{
  joyValX = analogRead(joyX);
  if (joyValX <= 341) {
    if (!(level >= 3)) {
      level +=1;
    }
  } else if (joyValX >= 682) {
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
  if (digitalRead(joyBtn) == LOW) {
    startGame();
  }
  delay(200);
}

void startGame()
{
  int snakeHeadX = 6;
  int snakeHeadY = 2;
  snakeDirection = 0;
  snakeLength = 1;
  initGrid();
  makeFood();
  while(inGrid(snakeHeadX, snakeHeadY)) {
    joyValX = analogRead(joyX);
    if (joyValX <= 341) {
      snakeDirection = 2; 
    } else if (joyValX >= 682) {
      snakeDirection = 4;
    }
    joyValY = analogRead(joyY);
    if (joyValY <= 341) {
      snakeDirection = 1;
    } else if (joyValY >= 682) {
      snakeDirection = 3;
    }
    if (snakeDirection == 1) {
      grid[snakeHeadX - 1][snakeHeadY] = 1;
      grid[snakeHeadX][snakeHeadY] = 0;
      snakeHeadX -= 1;  
    } else if (snakeDirection == 2) {
      grid[snakeHeadX][snakeHeadY + 1] = 1;
      grid[snakeHeadX][snakeHeadY] = 0;
      snakeHeadY += 1;  
    } else if (snakeDirection == 3) {
      grid[snakeHeadX + 1][snakeHeadY] = 1;
      grid[snakeHeadX][snakeHeadY] = 0;
      snakeHeadX += 1;  
    } else if (snakeDirection == 4) {
      grid[snakeHeadX][snakeHeadY - 1] = 1;
      grid[snakeHeadX][snakeHeadY] = 0;
      snakeHeadY -= 1;   
    }
    if (snakeHeadX == foodX && snakeHeadY == foodY) {
      makeFood();
      snakeLength += 1;
    }
    if (inGrid(snakeHeadX, snakeHeadY)) {
      drawGrid();
    }
    if (snakeDirection != 0) {
      lc.setLed(0, foodX, foodY, 1);
    }
    delay(500 / level);
  }
}

void makeFood()
{
  foodX = random(0, 8);
  foodY = random(0, 8);
}

bool inGrid(int x, int y)
{
  return ((x >= 0 && x <= 7) && (y >= 0 && y <= 7));
}

void initGrid()
{
  int i = 0;
  int j = 0;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      grid[i][j] = 0;
    }  
  }
  grid[6][2] = 1;
}

void drawGrid()
{
  int i = 0;
  int j = 0;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      lc.setLed(0, i, j, grid[i][j]);
    }  
  }
}

void printByte(byte character [])
{
  int i = 0;
  for(i = 0; i < 8; i++) {
    lc.setRow(0, i, character[i]);
  }
}
