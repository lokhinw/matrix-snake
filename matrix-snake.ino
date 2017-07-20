#include <LedControl.h>

boolean gameOver = false;

int snakeDirection; // 1 = UP; 2 = RIGHT; 3 = DOWN; 4 = LEFT

const int UP = 1;
const int RIGHT = 2;
const int DOWN = 3;
const int LEFT = 4;

int snakeX[64];
int snakeY[64];
int snakeLength = 1;

int level;

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
    gameOver = false;
    startGame();
  }
  
  delay(200);
}

void startGame()
{
  
  for (int i = 0; i < 64; i++) { 
    snakeX[i] = -1;
    snakeY[i] = -1;
  }
  
  snakeX[0] = 4;
  snakeY[0] = 4;
  
  snakeDirection = 0;
  snakeLength = 1;
  
  makeFood();
  
  while(gameOver == false) {
    lc.clearDisplay(0);
    
    joyValX = analogRead(joyX);
    if (joyValX <= 341 && snakeDirection != LEFT) {
      snakeDirection = RIGHT; 
    } else if (joyValX >= 682 && snakeDirection != RIGHT) {
      snakeDirection = LEFT;
    }
    
    joyValY = analogRead(joyY);
    if (joyValY <= 341 && snakeDirection != DOWN) {
      snakeDirection = UP;
    } else if (joyValY >= 682 && snakeDirection != UP) {
      snakeDirection = DOWN;
    }

    move(snakeDirection);
    
    if(!(inGrid(snakeX[0], snakeY[0]))) {
      gameOver = true;
    }
    
    ifSnakeEatFood();
    ifSnakeEatItself();
    
    drawSnake();
    drawFood();
    
    delay(500 / level);
  }
}

void move(int dir) {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  if (dir == UP) {
      snakeY[0]--;
    } else if (dir == RIGHT) {
      snakeX[0]++;
    } else if (dir == DOWN) {
      snakeY[0]++;
    } else if (dir == LEFT) {
      snakeX[0]--;
    }
}

void ifSnakeEatFood() {
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    makeFood();
  }
}

void ifSnakeEatItself() {
  for (int i = 1; i < snakeLength - 1; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver = true;
    }
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

void drawSnake() {
  for (int i = 0; i < snakeLength; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }
}

void drawFood() {
  lc.setLed(0, foodY, foodX, true);
}

void printByte(byte character [])
{
  for(int i = 0; i < 8; i++) {
    lc.setRow(0, i, character[i]);
  }
}
