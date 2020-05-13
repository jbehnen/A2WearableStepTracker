#include "Oled.h"
#include <Arduino.h>
#include <Wire.h>
#include <string> 
#include <sstream> 

// Adapted from the demo code referenced at  https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples

Oled :: Oled(int screenWidth, int screenHeight, int oledResetPin) : 
  _screenWidth(screenWidth), _screenHeight(screenHeight), _oledResetPin(oledResetPin) {
  _display = Adafruit_SSD1306(screenWidth, screenHeight, &Wire, oledResetPin);
}

static const unsigned char PROGMEM Oled_Happydog_data[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x02,
  0x02, 0x00, 0x00, 0x00, 0x0C, 0x01, 0x80, 0x00, 0x00, 0x10, 0x00, 0x40, 0x00, 0x00, 0x30, 0x00,
  0x60, 0x00, 0x00, 0x73, 0xDE, 0x70, 0x00, 0x00, 0x72, 0x52, 0x70, 0x00, 0x00, 0xFA, 0x52, 0xF8,
  0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0xF8, 0x70, 0xF8, 0x00,
  0x00, 0xF8, 0x20, 0xF8, 0x00, 0x38, 0xF9, 0x24, 0xF8, 0x00, 0x0C, 0xF9, 0x24, 0xF8, 0x00, 0x06,
  0xF8, 0xF8, 0xF8, 0x00, 0x03, 0xF8, 0x00, 0xFF, 0x80, 0x01, 0x7C, 0x01, 0xF0, 0x78, 0x01, 0x73,
  0x06, 0x70, 0x07, 0x03, 0x22, 0xF8, 0x20, 0x00, 0x86, 0x04, 0x00, 0x00, 0x00, 0x5C, 0x08, 0x00,
  0x00, 0x00, 0x30, 0x10, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00,
  0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00,
  0x80, 0x01, 0xC8, 0x00, 0x07, 0x00, 0x01, 0x3C, 0x00, 0x79, 0x00, 0x01, 0x0B, 0xFF, 0x91, 0x00,
  0x01, 0x08, 0x00, 0x11, 0x00, 0x01, 0x08, 0x00, 0x11, 0x00, 0x01, 0x08, 0x00, 0x11, 0x00, 0x01,
  0x08, 0x00, 0x11, 0x00, 0x01, 0x08, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static const unsigned char PROGMEM Oled_Bone_Solid_data[] = {
  0x78, 0x01, 0xe0, 0xfc, 0x03, 0xf0, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xe0, 0x3f, 0xff, 0xc0, 0x3f, 
  0xff, 0xc0, 0x7f, 0xff, 0xe0, 0xff, 0xff, 0xf0, 0xfc, 0x03, 0xf0, 0x78, 0x01, 0xe0
};

static const unsigned char PROGMEM Oled_Bone_Hollow_data[] = {
  0x78, 0x01, 0xe0, 0x84, 0x02, 0x10, 0x83, 0xfc, 0x10, 0x40, 0x00, 0x20, 0x20, 0x00, 0x40, 0x20, 
  0x00, 0x40, 0x40, 0x00, 0x20, 0x83, 0xfc, 0x10, 0x84, 0x02, 0x10, 0x78, 0x01, 0xe0
};

static const unsigned char PROGMEM Oled_Battery_data[] = {
  0x0c, 0x00, 0x3f, 0x00, 0x21, 0x00, 0x29, 0x00, 0x25, 0x00, 0x2d, 0x00, 0x29, 0x00, 0x25, 0x00, 
  0x21, 0x00, 0x3f, 0x00
};

static const unsigned char PROGMEM Oled_Foot_data[] = {
  0x00, 0x00, 0x03, 0x00, 0x0b, 0x00, 0x10, 0x00, 0x27, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x07, 0x00, 
  0x03, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM Oled_Heart_data[] = {
  0x61, 0x80, 0xf3, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0x7f, 0x80, 0x3f, 0x00, 
  0x1e, 0x00, 0x0c, 0x00
};

const unsigned char* Oled::HAPPYDOG = Oled_Happydog_data;
const unsigned char* Oled::BONE_SOLID = Oled_Bone_Solid_data;
const unsigned char* Oled::BONE_HOLLOW = Oled_Bone_Hollow_data;
const unsigned char* Oled::BATTERY = Oled_Battery_data;
const unsigned char* Oled::FOOT = Oled_Foot_data;
const unsigned char* Oled::HEART = Oled_Heart_data;

const int Oled::DISPLAY_WIDTH = 128;
const int Oled::DISPLAY_HEIGHT = 64;

const int Oled::X_OFFSET = 20;
const int Oled::ICON_SIDE = 10;
const int Oled::ICON_Y_SPACING = 5;
const int Oled::ICON_TEXT_X_SPACING = 5;
const int Oled::ICON_TEXT_Y_SPACING = 1;

const int Oled::HAPPYDOG_SIDE = 40;
const int Oled::HAPPYDOG_JUMP = 10;
const int Oled::HAPPYDOG_X = DISPLAY_WIDTH  - HAPPYDOG_SIDE - X_OFFSET;
const int Oled::HAPPYDOG_Y = (DISPLAY_HEIGHT - HAPPYDOG_SIDE) / 2;

const int Oled::FOOT_Y = 0;
const int Oled::BATTERY_Y = ICON_SIDE + ICON_Y_SPACING;
const int Oled::BONE_SOLID_Y = DISPLAY_HEIGHT - ICON_SIDE * 2 - ICON_Y_SPACING;
const int Oled::BONE_HOLLOW_Y = DISPLAY_HEIGHT - ICON_SIDE;
const int Oled::HEART_Y = DISPLAY_HEIGHT - ICON_SIDE;

const int Oled::HEART_X = HAPPYDOG_X - 1;
const int Oled::HEART_SPACING = 5;

const int Oled::BONE_WIDTH = 20;
const int Oled::BONE_HEIGHT = 10;

void Oled :: setup() {
  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  _display.display();
}

void Oled :: update(int jumpHeight, 
                int steps, 
                int totalBones, 
                int remainingBones, 
                int batteryPercentage,
                bool giveTreat) {
  _display.clearDisplay();
  
  // Battery
  drawIconAndLabel(BATTERY, BATTERY_Y, ICON_SIDE, ICON_SIDE, batteryPercentage);
  
  // Steps
  drawIconAndLabel(FOOT, FOOT_Y, ICON_SIDE, ICON_SIDE, steps);
  
  // Solid bone
  drawIconAndLabel(BONE_SOLID, BONE_SOLID_Y, BONE_WIDTH, BONE_HEIGHT, remainingBones);
  
  // Hollow bone
  drawIconAndLabel(BONE_HOLLOW, BONE_HOLLOW_Y, BONE_WIDTH, BONE_HEIGHT, totalBones);
    
  // Happy dog
  drawHappyDog(jumpHeight);
  
  // Treats!
  if (giveTreat) {
    drawHeart(HEART_X);
    drawHeart(HEART_X + ICON_SIDE + HEART_SPACING);
    drawHeart(HEART_X + 2 * (ICON_SIDE + HEART_SPACING));
  }
  
  _display.display();
}

void Oled :: drawString(std::string message, int x, int y) {
  _display.setTextSize(1);      // Normal 1:1 pixel scale
  _display.setTextColor(SSD1306_WHITE); // Draw white text
  _display.setCursor(x, y);
  _display.cp437(true);         // 'Code Page 437' font
  for(int16_t i=0; i<message.length(); i++) {
    _display.write(message[i]);
  }
}

void Oled :: drawHappyDog(int jumpHeight) {
  _display.drawBitmap(
             HAPPYDOG_X,  
             HAPPYDOG_Y + jumpHeight,
             HAPPYDOG,
             HAPPYDOG_SIDE, 
             HAPPYDOG_SIDE,
             1);
}

void Oled :: drawIconAndLabel(const unsigned char* bitmap, 
                              int y, 
                              int width, 
                              int height, 
                              int label) {
  _display.drawBitmap(
             X_OFFSET,  
             y,
             bitmap,
             width, 
             height,
             1);
  drawString(
    intToString(label), 
    X_OFFSET + width + ICON_TEXT_X_SPACING, 
    y + ICON_TEXT_Y_SPACING);
}

void Oled :: drawHeart(int x) {
  _display.drawBitmap(
             x,  
             HEART_Y,
             HEART,
             ICON_SIDE, 
             ICON_SIDE,
             1);
}

// Private 

std::string Oled :: intToString(int i) {
  std::stringstream ss;
  ss << i;
  return ss.str();
}

