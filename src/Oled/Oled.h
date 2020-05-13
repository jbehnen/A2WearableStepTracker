#ifndef OLED_H
#define OLED_H

#include <Adafruit_SSD1306.h>
#include <string> 

/**
* Updates the OLED display.
* Optimized for a 128 x 64 screen.
*   
* By Julia Behnen
* 
* Adapted from the demo code referenced at  https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples
*/ 
class Oled {
  private:
    // Bitmaps
    // https://stackoverflow.com/questions/11367141/how-to-declare-and-initialize-a-static-const-array-as-a-class-member
    static const unsigned char* HAPPYDOG;
    static const unsigned char* BONE_SOLID;
    static const unsigned char* BONE_HOLLOW;
    static const unsigned char* BATTERY;
    static const unsigned char* FOOT;
    static const unsigned char* HEART;
    
    static const int DISPLAY_WIDTH;
    static const int DISPLAY_HEIGHT;
    
    static const int X_OFFSET;
    static const int ICON_SIDE;
    static const int ICON_Y_SPACING;
    static const int ICON_TEXT_X_SPACING;
    static const int ICON_TEXT_Y_SPACING;
    
    static const int HAPPYDOG_SIDE;
    static const int HAPPYDOG_JUMP;
    static const int HAPPYDOG_X;
    static const int HAPPYDOG_Y;
    
    static const int BONE_SOLID_Y;
    static const int BONE_HOLLOW_Y;
    static const int BATTERY_Y;
    static const int FOOT_Y;
    static const int HEART_Y;
    
    static const int HEART_X;
    static const int HEART_SPACING;
    
    static const int BONE_WIDTH;
    static const int BONE_HEIGHT;
    
    const int _screenWidth;
    const int _screenHeight;
    const int _oledResetPin;
	
    Adafruit_SSD1306 _display; // OLED
    
    std::string intToString(int i);
    
  public:
    // Constructor
    Oled(int screenWidth, int screenHeight, int oledResetPin);
	
    /*
     * Sets up the class. Must be called before any other methods.
     */
    void setup();
	  
    /*
     * Updates the OLED display.
     */
    void update(int jumpHeight, 
                int steps, 
                int totalBones, 
                int remainingBones, 
                int batteryPercentage,
                bool giveTreat);
    
    /*
     * Writes the given message starting at the given
     * (x, y) coordinate.
     */
    void drawString(std::string message, int x, int y);
    
    /*
     * Draws the HAPPYDOG bitmap adjusted upward by the
     * number of pixels specified by jumpHeight.
     */
    void drawHappyDog(int jumpHeight);
    
    /*
     * Draws given bitmap icon with the given y coordinate,
     * width, and height (and x coordinate X_OFFSET);
     * followed by an integer label. 
     */
    void drawIconAndLabel(const unsigned char* bitmap, 
                          int y, 
                          int width, 
                          int height, 
                          int label);
    
    /*
     * Draws the HEART bitmap at the coordinate (x, HEART_Y).
     */
    void drawHeart(int x);
};

#endif