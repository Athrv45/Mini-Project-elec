#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>

// OLED Setup
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);
#define OLED_I2C_ADDR 0x3C

// DHT11 Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// BMP280 Setup
Adafruit_BMP280 bmp; 

// MQ135 Setup
#define MQ135_PIN A1  // Analog output of MQ135

void setup() {
    Serial.begin(9600);
    dht.begin();
    if (!bmp.begin(0x76)) {
        Serial.println("BMP280 not found!");
        while (1);
    }

    display.begin(SH1106_I2C_ADDRESS, OLED_I2C_ADDR);
    display.clearDisplay();
}

void loop() {
    // Read DHT11 Sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Read BMP280 Sensor
    float pressure = bmp.readPressure() / 100.0F;
    float altitude = bmp.readAltitude(1013.25);

    // Read MQ135 Sensor (Air Quality)
    int mq135Value = analogRead(MQ135_PIN);
    float co2_ppm = mq135Value * 0.1;  // Basic calibration
    float aqi = mq135Value * 0.05;     // Approximate AQI
    float alcohol = mq135Value * 0.02; // Approximate Alcohol concentration

    // Display on OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Display DHT11 Data
    display.setCursor(0, 0);
    display.print("Temp: "); display.print(temperature); display.print(" C");

    display.setCursor(0, 10);
    display.print("Hum: "); display.print(humidity); display.print(" %");

    // Display BMP280 Data
    display.setCursor(0, 20);
    display.print("Press: "); display.print(pressure); display.print(" hPa");

    display.setCursor(0, 30);
    display.print("Alt: "); display.print(altitude); display.print(" m");

    // Display MQ135 Data
    display.setCursor(0, 40);
    display.print("CO2: "); display.print(co2_ppm); display.print(" ppm");

    display.setCursor(0, 50);
    display.print("AQI: "); display.print(aqi);

    display.setCursor(0, 60);
    display.print("Alcohol: "); display.print(alcohol);

    display.display();
    delay(2000);  // Update every 2 seconds
}
