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

void setup() {
    Serial.begin(115200);
    Wire.begin();

    u8g2.begin();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(10, 10, "Initializing...");
    } while (u8g2.nextPage());
    delay(2000);

    dht.begin();
    Serial.println("DHT11 Initialized!");

    if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
        Serial.println("BMP280 Not Found!");
        u8g2.firstPage();
        do {
            u8g2.drawStr(10, 10, "BMP280 Error!");
        } while (u8g2.nextPage());
        while (1);
    }
    Serial.println("BMP280 Initialized!");

    u8g2.firstPage();
    do {
        u8g2.drawStr(10, 10, "Sensors Ready!");
    } while (u8g2.nextPage());
    delay(2000);
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float p = bmp.readPressure() / 100.0F;
    float a = bmp.readAltitude(1013.25);
    float co2_ppm = mq135.getPPM();
    int aqi = map(co2_ppm, 400, 5000, 0, 500);  

    if (isnan(h) || isnan(t) || isnan(p) || isnan(a)) {
        Serial.println("Sensor Read Error!");
        u8g2.clearBuffer();
        u8g2.firstPage();
        do {
            u8g2.drawStr(10, 10, "Sensor Error!");
        } while (u8g2.nextPage());
        delay(2000);
        return;
    }

    // Show Temperature & Humidity
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.setCursor(10, 10);
        u8g2.print("Temp: "); u8g2.print(t); u8g2.print(" C");

        u8g2.setCursor(10, 25);
        u8g2.print("Hum: "); u8g2.print(h); u8g2.print(" %");
    } while (u8g2.nextPage());
    delay(4000);

    // Show Pressure & Altitude
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.setCursor(10, 10);
        u8g2.print("Pressure: "); u8g2.print(p); u8g2.print(" hPa");

        u8g2.setCursor(10, 25);
        u8g2.print("Altitude: "); u8g2.print(a); u8g2.print(" m");
    } while (u8g2.nextPage());
    delay(4000);

    // Show CO2 & AQI
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
        u8g2.setCursor(10, 10);
        u8g2.print("CO2: "); u8g2.print(co2_ppm); u8g2.print(" PPM");

        u8g2.setCursor(10, 25);
        u8g2.print("AQI: "); u8g2.print(aqi);
    } while (u8g2.nextPage());
    delay(4000);
}
