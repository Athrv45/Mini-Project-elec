#include <MQ135.h>  // Include MQ135 library

#define MQ135_PIN A0  // Connect MQ135 analog output to A0

MQ135 mq135_sensor(MQ135_PIN);

void setup() {
    Serial.begin(9600);
    Serial.println("MQ-135 Sensor Test - Separate Readings");
    delay(2000); // Allow sensor to stabilize
}

void loop() {
    float air_quality = analogRead(MQ135_PIN);  // Raw sensor value (0-1023)
    
    // Convert raw value to estimated ppm levels
    float co2_ppm = mq135_sensor.getPPM();  // Approximate CO2 concentration
    float nh3_level = air_quality * 0.05;   // Approximate NH3/Alcohol/Benzene

    Serial.print("Raw Air Quality Value: ");
    Serial.print(air_quality);
    Serial.println(" / 1023");

    Serial.print("Estimated CO2 Level: ");
    Serial.print(co2_ppm);
    Serial.println(" ppm");

    Serial.print("Estimated NH3/Alcohol/Benzene Level: ");
    Serial.print(nh3_level);
    Serial.println(" ppm");

    Serial.println("-----------------------------");
    delay(2000);  // Wait for 2 seconds before the next reading
}
