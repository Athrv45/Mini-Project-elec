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

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    u8g2.begin();

    displayMessage("Weather Station\nStarted!");
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
        displayMessage("Sensor Error!");
        delay(2000);
        return;
    }

    displaySensorData("\xB0C", "Temp", t, " %", "Hum", h);
    displaySensorData("hPa", "Press", p, " m", "Alt", a);
    displaySensorData("PPM", "CO2", co2_ppm, " AQI", "AQI", aqi);
}

void displayMessage(const char* message) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.firstPage();
    do {
        u8g2.drawStr(20, 30, message);
    } while (u8g2.nextPage());
    delay(2000);
}

void displaySensorData(const char* unit1, const char* label1, float value1, const char* unit2, const char* label2, float value2) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.firstPage();
    do {
        u8g2.setCursor(5, 15);
        u8g2.print(label1); u8g2.print(": "); u8g2.print(value1); u8g2.print(unit1);

        u8g2.setCursor(5, 35);
        u8g2.print(label2); u8g2.print(": "); u8g2.print(value2); u8g2.print(unit2);

        u8g2.drawHLine(0, 45, 128); // Separator line
    } while (u8g2.nextPage());
    delay(4000);
}
