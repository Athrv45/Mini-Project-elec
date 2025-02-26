#include <Wire.h>
#include <U8g2lib.h>

// Initialize SH1106 OLED (I2C)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  u8g2.begin();  // Start OLED display
}

void loop() {
  u8g2.clearBuffer();  // Clear display buffer
  
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Set a readable font
  u8g2.drawStr(10, 20, "Hello World!");  // Print text

  // Draw a basic smiley face
  u8g2.drawCircle(50, 50, 10, U8G2_DRAW_ALL); // Face outline
  u8g2.drawDisc(46, 46, 1, U8G2_DRAW_ALL); // Left Eye
  u8g2.drawDisc(54, 46, 1, U8G2_DRAW_ALL); // Right Eye
  u8g2.drawArc(50, 50, 6, 200, 340); // Smiley Mouth (Adjusted)

  u8g2.sendBuffer();  // Send buffer to display
  delay(1000);  // Wait 1 sec
}
