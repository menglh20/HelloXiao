#include <Arduino.h>

static const int ANALOG_PIN = A0;

static const uint32_t BAUD_RATE = 115200;

static const float ADC_MAX = 4095.0;
static const float VREF = 3.3;

void setup() {
  Serial.begin(BAUD_RATE);
  delay(300);

  Serial.println();
  Serial.println("=== ESP32-C3 A0 ADC Voltage Reader ===");

}

void loop() {
  int adcValue = analogRead(ANALOG_PIN);

  float voltage = (adcValue / ADC_MAX) * VREF;

  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print("  Voltage: ");
  Serial.print(voltage, 3);
  Serial.println(" V");

  delay(200);
}
