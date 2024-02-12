// ----------------------------
// Default Arduino Libraries
// ----------------------------
#include <SPI.h>

// ----------------------------
// Additional Libraries - Installation required 
// ----------------------------

// ----------------------------
// Display
// The Display pins are defined in the UserSetup.h
// Make sure to copy/edit the UserSetup.h file into the library
// ----------------------------
#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

// ----------------------------
// Touch Screen
// ----------------------------
#include "TAMC_GT911.h"

// SPI pins
#define TOUCH_SDA  33
#define TOUCH_SCL  32
#define TOUCH_INT 25
#define TOUCH_RST 26
#define TOUCH_WIDTH  320
#define TOUCH_HEIGHT 240

// ----------------------------

// color definitions:
   #define BLACK   0x0000
   #define WHITE   0xFFFF
// ----------------------------

// initialize variables:
  float x_edge = 0;
  float y_edge = 0;
  float radius = 120;
  float angle_circle = 0;
  int j = 0;
// ----------------------------

// initialize library definitions:
SPIClass mySpi = SPIClass(VSPI);
TFT_eSPI tft = TFT_eSPI();
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

void setup() {
  Serial.begin(115200);

  // Start the SPI for the touch screen and init the TS library
  //mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  tp.begin();
  tp.setRotation(ROTATION_RIGHT);

  // Start the tft display and set it to black
  tft.init();
  tft.setRotation(1); //This is the display in landscape

  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);

  int x = 320 / 2; // center of display
  int y = 100;
  int fontSize = 2;
  tft.drawCentreString("Touch Screen to Start", x, y, fontSize);
  tft.drawRect (0,0,320,240,WHITE);
}

void printTouchToSerial(int point_x, int point_y, int point_size) {
  Serial.print("Size = ");
  Serial.print(point_size);
  Serial.print(", x = ");
  Serial.print(point_x);
  Serial.print(", y = ");
  Serial.print(point_y);
  Serial.println();
}

void printTouchToDisplay(int point_x, int point_y, int point_size) {

  // Clear screen first
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  int x = 320 / 2; // center of display
  int y = 100;
  int fontSize = 2;

  String temp = "Size = " + String(point_size);
  tft.drawCentreString(temp, x, y, fontSize);

  y += 16;
  temp = "X = " + String(point_x);
  tft.drawCentreString(temp, x, y, fontSize);

  y += 16;
  temp = "Y = " + String(point_y);
  tft.drawCentreString(temp, x, y, fontSize);
}


void drawCircle(int point_x, int point_y, int point_size) {
  radius = float (point_size);
  for (j=0; j<360; j++) {
    angle_circle = (j * 0.01745331);        // angle expressed in radians - 1 degree = 0,01745331 radians
    x_edge = (float(point_x) + (radius*cos(angle_circle)));
    y_edge = (float(point_y) + (radius*sin(angle_circle))); 
    tft.drawPixel(x_edge,y_edge, WHITE); 
  }
}


void loop() {
  tp.read();
  if (tp.isTouched){
    for (int i=0; i<tp.touches; i++){
      int point_size = tp.points[0].size;
      int point_x = tp.points[0].x;
      int point_y = tp.points[0].y;
      printTouchToSerial(point_x, point_y, point_size);
      printTouchToDisplay(point_x, point_y, point_size);
      drawCircle(tp.points[i].x, tp.points[i].y, tp.points[i].size); // draw all touch points 5 max
      delay(100);
    }
  }
}


