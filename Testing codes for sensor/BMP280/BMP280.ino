#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_BMP280.h>

// OLED Setup
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);
#define OLED_I2C_ADDR 0x3C

// BMP280 Setup
Adafruit_BMP280 bmp;

void setup() {
    Serial.begin(9600);
    
    // Try both addresses
    if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
        Serial.println("BMP280 not found! Check wiring.");
        while (1);
    } else {
        Serial.println("BMP280 detected!");
    }

    display.begin(SH1106_I2C_ADDRESS, OLED_I2C_ADDR);
    display.clearDisplay();
}

void loop() {
    float pressure = bmp.readPressure() / 100.0F;
    float altitude = bmp.readAltitude(1013.25);

    Serial.print("Pressure: "); Serial.print(pressure); Serial.print(" hPa, ");
    Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");

    display.clearDisplay();
    display.setTextSize(1.8);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Pressure: "); display.print(pressure); display.print(" hPa");

    display.setCursor(0, 30);
    display.print("Altitude: "); display.print(altitude); display.print(" m");

    display.display();
    delay(2000);
}
