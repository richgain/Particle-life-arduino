// Hardware-specific library
#include <SPI.h>
#include <TFT_eSPI.h>
#include <math.h>

// Invoke custom library
TFT_eSPI tft = TFT_eSPI();

// Size of our cells
#define CELLXY 2
// Define radius of influence
#define RADIUS 80
// Size of a set of particles
#define SETSIZE 50
// Resolution of our display
#define SCREENX 320
#define SCREENY 170
// Compute the needed size of our grid
#define GRIDX SCREENX / CELLXY
#define GRIDY SCREENY / CELLXY
// Target: 60 FPS
#define GEN_DELAY (1000 / 60)
// Maximum number of generations before resetting
#define NUMGEN 200

// define the data structure of an individual particle
struct atom {
  int16_t x;
  int16_t y;
  float vx;
  float vy;
  uint16_t colour;
};

// Declare the atoms array with a fixed size
atom a;
atom b;
atom blue[SETSIZE];
atom yellow[SETSIZE];
atom red[SETSIZE];
atom green[SETSIZE];

float RR;
float RG;
float RB;
float RY;

float GR;
float GG;
float GB;
float GY;

float BRed;
float BG;
float BB;
float BY;

float YR;
float YG;
float YB;
float YY;

void setup(void) {
  pinMode(15, OUTPUT); // to boot with battery...
  digitalWrite(15,1);  // and/or power from 5v rail instead of USB
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  randomSeed(analogRead(A0));
};

void loop() {
// Create the particles data arrays
  for (uint16_t i = 0; i < SETSIZE; i++) {
    blue[i] = {random(10/CELLXY, 310/CELLXY), random(10/CELLXY, 160/CELLXY), 0, 0, TFT_BLUE};
  }
  for (uint16_t i = 0; i < SETSIZE; i++) {
    yellow[i] = {random(10/CELLXY, 310/CELLXY), random(10/CELLXY, 160/CELLXY), 0, 0, TFT_YELLOW};
  }
  for (uint16_t i = 0; i < SETSIZE; i++) {
    red[i] = {random(10/CELLXY, 310/CELLXY), random(10/CELLXY, 160/CELLXY), 0, 0, TFT_RED};
  }
  for (uint16_t i = 0; i < SETSIZE; i++) {
    green[i] = {random(10/CELLXY, 310/CELLXY), random(10/CELLXY, 160/CELLXY), 0, 0, TFT_GREEN};
  }
  
  // Create the new rules
 RR = random(-1,1);
 RG = random(-1,1);
 RB = random(-1,1);
 RY = random(-1,1);
 GR = random(-1,1);
 GG = random(-1,1);
 GB = random(-1,1);
 GY = random(-1,1);
 BRed = random(-1,1);
 BG = random(-1,1);
 BB = random(-1,1);
 BY = random(-1,1);
 YR = random(-1,1);
 YG = random(-1,1);
 YB = random(-1,1);
 YY = random(-1,1);

  tft.fillScreen(TFT_BLACK);
  drawGrid();
  delay(GEN_DELAY);

  for (uint16_t gen = 0; gen < NUMGEN; gen++) {
    drawGrid();
    delay(GEN_DELAY);
  }
};

void drawGrid(void) {
//  rule(green, green, -0.32);
//  rule(green, red, -0.17);
//  rule(green, yellow, 0.34);
//  rule(red, red, -0.1);
//  rule(red, green, -0.34);
//  rule(yellow, yellow, 0.15);
//  rule(yellow, green, -0.2);

  rule(green, green, GG);
  rule(green, red, GR);
  rule(green, yellow, GY);
  rule(green, blue, GB);
  rule(red, green, RG);
  rule(red, red, RR);
  rule(red, yellow, RY);
  rule(red, blue, RB);
  rule(yellow, green, YG);
  rule(yellow, red, YR);
  rule(yellow, yellow, YY);
  rule(yellow, blue, YB);
  rule(blue, green, BG);
  rule(blue, red, BRed);
  rule(blue, yellow, BY);
  rule(blue, blue, BB);

  tft.fillScreen(TFT_BLACK);

  for (uint16_t i = 0; i < SETSIZE; i++) {
      tft.fillRect(CELLXY * blue[i].x, CELLXY * blue[i].y, CELLXY, CELLXY, TFT_BLUE);
  }
  for (uint16_t i = 0; i < SETSIZE; i++) {
      tft.fillRect(CELLXY * yellow[i].x, CELLXY * yellow[i].y, CELLXY, CELLXY, TFT_YELLOW);
  }
  for (uint16_t i = 0; i < SETSIZE; i++) {
      tft.fillRect(CELLXY * red[i].x, CELLXY * red[i].y, CELLXY, CELLXY, TFT_RED);
  }
  for (uint16_t i = 0; i < SETSIZE; i++) {
      tft.fillRect(CELLXY * green[i].x, CELLXY * green[i].y, CELLXY, CELLXY, TFT_GREEN);
  }
};

void rule(atom atoms1[], atom atoms2[], float g) {
  for (uint16_t i = 0; i < SETSIZE; i++) {
    a = atoms1[i];
    float fx = 0;
    float fy = 0;
    for (uint16_t j = 0; j < SETSIZE; j++) {
      b = atoms2[j];
      int dx = a.x - b.x;
      int dy = a.y - b.y;
      float d = sqrt(dx * dx + dy * dy);
      if (d > 0 && d < RADIUS) {
        float force = (g * 1) / d;
        fx += force * dx;
        fy += force * dy;        
      }
    }
    a.vx = (a.vx + fx) * 0.5; 
    a.vy = (a.vy + fy) * 0.5; 
    a.x += a.vx;
    a.y += a.vy;
    if (a.x <= 0 || a.x >= GRIDX-1) { a.vx *= -1; }
    if (a.y <= 0 || a.y >= GRIDY-1) { a.vy *= -1; }
    atoms1[i] = a;
  }
};
