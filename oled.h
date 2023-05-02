#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

Adafruit_SSD1306 display_L(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display_R(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool init_oled(){

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display_L.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }

  return true;
}

void testdrawstyles(String print_text, uint8_t col, uint8_t row, uint8_t text_size) {


  display_L.setTextSize(text_size);             // Normal 1:1 pixel scale
  display_L.setTextColor(SSD1306_WHITE);        // Draw white text
  display_L.setCursor(col,row);             // Start at top-left corner
  display_L.println(print_text);

  // display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  // display.println(3.141592);

  // display.setTextSize(2);             // Draw 2X-scale text
  // display.setTextColor(SSD1306_WHITE);
  // display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  // display.display();
  // delay(2000);
}

void testdrawbitmap(const unsigned char* bmp, uint8_t col, uint8_t row) {


  display_L.drawBitmap(
    col,row,
    bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);

  // delay(1000);
}
