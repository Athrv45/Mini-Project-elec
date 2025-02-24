#include <DHT.h>

#define DHTPIN 2       // DHT11 data pin connected to D2
#define DHTTYPE DHT11  // Using DHT11 sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    Serial.println("DHT11 Sensor Test");
    dht.begin();
}

void loop() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temp) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C  Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    delay(2000); // Wait 2 seconds before next reading
}