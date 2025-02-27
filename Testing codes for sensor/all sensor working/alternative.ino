#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <U8g2lib.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp;

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
    Serial.begin(115200);
    Wire.begin();

    u8g2.begin();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_6x12_tf);
        u8g2.drawStr(15, 20, "SMART WEATHER STATION");
    } while (u8g2.nextPage());
    delay(3000);

    dht.begin();
    if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
        Serial.println("BMP280 Not Found!");
        u8g2.firstPage();
        do {
            u8g2.drawStr(10, 20, "BMP280 ERROR!");
        } while (u8g2.nextPage());
        while (1);
    }
    Serial.println("Sensors Ready!");
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float p = bmp.readPressure() / 100.0F;
    float a = bmp.readAltitude(1013.25);
    int aqi = random(50, 300); // Placeholder AQI Value

    if (isnan(h) || isnan(t) || isnan(p) || isnan(a)) {
        Serial.println("Sensor Read Error!");
        u8g2.firstPage();
        do {
            u8g2.drawStr(10, 20, "SENSOR ERROR!");
        } while (u8g2.nextPage());
        delay(2000);
        return;
    }

    // Show Temperature & Humidity
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_6x12_tf);
        u8g2.drawStr(5, 15, "\xF8 Temp: "); u8g2.setCursor(50, 15); u8g2.print(t); u8g2.print(" C");
        u8g2.drawStr(5, 35, "\xF7 Hum: "); u8g2.setCursor(50, 35); u8g2.print(h); u8g2.print(" %");
    } while (u8g2.nextPage());
    delay(4000);

    // Show Pressure & Altitude
    u8g2.firstPage();
    do {
        u8g2.drawStr(5, 15, "Pressure:"); u8g2.setCursor(65, 15); u8g2.print(p); u8g2.print(" hPa");
        u8g2.drawStr(5, 35, "Altitude:"); u8g2.setCursor(65, 35); u8g2.print(a); u8g2.print(" m");
    } while (u8g2.nextPage());
    delay(4000);

    // Show AQI
    u8g2.firstPage();
    do {
        u8g2.drawStr(5, 15, "AQI Level: "); u8g2.setCursor(70, 15); u8g2.print(aqi);
        int barWidth = map(aqi, 0, 500, 0, 100);
        u8g2.drawBox(10, 30, barWidth, 10);
    } while (u8g2.nextPage());
    delay(4000);
}
