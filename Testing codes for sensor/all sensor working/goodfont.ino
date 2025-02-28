#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <U8g2lib.h>
#include <MQ135.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp;

#define MQ135_PIN A0
MQ135 mq135(MQ135_PIN);

// Use PAGE MODE (Saves RAM)
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const float SEA_LEVEL_PRESSURE_HPA = 1010.0; // Local pressure in Ahmednagar

void setup() {
    Serial.begin(115200);
    Wire.begin();

    u8g2.begin();
    displayMessage("Weather Station Started");
    delay(2000);

    dht.begin();
    Serial.println("DHT11 Initialized!");

    if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
        Serial.println("BMP280 Not Found!");
        displayMessage("BMP280 Error!");
        while (1);
    }
    Serial.println("BMP280 Initialized!");

    displayMessage("Sensors Ready!");
    delay(2000);

    // Show a cloud icon for effect
    displayCloud();
    delay(2000);
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float p = bmp.readPressure() / 100.0F;
    float a = bmp.readAltitude(SEA_LEVEL_PRESSURE_HPA);
    float co2_ppm = mq135.getPPM();
    int aqi = map(co2_ppm, 400, 5000, 50, 500); // Basic mapping for AQI

    if (isnan(h) || isnan(t) || isnan(p) || isnan(a)) {
        Serial.println("Sensor Read Error!");
        displayMessage("Sensor Error!");
        delay(2000);
        return;
    }

    // Show Temperature & Humidity
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.setCursor(10, 15);
        u8g2.print("Temp: "); u8g2.print(t); u8g2.print(" C");

        u8g2.setCursor(10, 30);
        u8g2.print("Hum: "); u8g2.print(h); u8g2.print(" %");

        u8g2.drawHLine(0, 45, 128); // Line separator
    } while (u8g2.nextPage());
    delay(4000);

    // Show Pressure & Altitude
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.setCursor(10, 15);
        u8g2.print("Pressure: "); u8g2.print(p); u8g2.print(" hPa");

        u8g2.setCursor(10, 30);
        u8g2.print("Altitude: "); u8g2.print(a); u8g2.print(" m");

        u8g2.drawHLine(0, 45, 128); // Line separator
    } while (u8g2.nextPage());
    delay(4000);

    // Show CO2 & AQI
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.setCursor(10, 15);
        u8g2.print("CO2: "); u8g2.print(co2_ppm); u8g2.print(" PPM");

        u8g2.setCursor(10, 30);
        u8g2.print("AQI: "); u8g2.print(aqi);

        u8g2.drawHLine(0, 45, 128); // Line separator
    } while (u8g2.nextPage());
    delay(4000);

    // Display concluding message
    displayMessage("Stay Safe and Healthy");
    delay(2000);
}

// Function to display a message on OLED
void displayMessage(const char* message) {
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(10, 30, message);
    } while (u8g2.nextPage());
}

// Function to display a simple cloud icon
void displayCloud() {
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.drawCircle(40, 30, 10);  // Left cloud
        u8g2.drawCircle(50, 25, 12);  // Middle cloud
        u8g2.drawCircle(60, 30, 10);  // Right cloud
        u8g2.drawBox(40, 30, 30, 10); // Bottom of cloud
    } while (u8g2.nextPage());
}
