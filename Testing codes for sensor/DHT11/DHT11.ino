#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <DHT.h>

// OLED Setup
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);
#define OLED_I2C_ADDR 0x3C

// DHT11 Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();

    display.begin(SH1106_I2C_ADDRESS, OLED_I2C_ADDR);
    display.clearDisplay();
}

void loop() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    Serial.print("Temp: "); Serial.print(temperature); Serial.print(" C, ");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");

    display.clearDisplay();
    display.setTextSize(1.7);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Temperture: "); display.print(temperature); display.print(" C");

    display.setCursor(0, 30);
    display.print("Humidity: "); display.print(humidity); display.print(" %");

    display.display();
    delay(2000);
}