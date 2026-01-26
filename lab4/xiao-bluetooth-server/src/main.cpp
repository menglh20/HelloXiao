#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <stdlib.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
unsigned long previousMillis = 0;
const long interval = 1000;

// TODO: add new global variables for your sensor readings and processed data
const int TRIG_PIN = D2;  // output pin
const int ECHO_PIN = D3;  // input pin

float rawDistanceCm = 0.0f;
float denoisedDistanceCm = 0.0f;

const int MA_WINDOW = 5;                 // moving average window size
float maBuffer[MA_WINDOW] = {0};
int maIndex = 0;
bool maFilled = false;

// TODO: Change the UUID to your own (any specific one works, but make sure they're different from others'). You can generate one here: https://www.uuidgenerator.net/
#define SERVICE_UUID        "89c6a56a-3c49-403e-9694-3045df60eb85"
#define CHARACTERISTIC_UUID "0b23ea84-8a96-45e5-b941-9f3cb7bbe557"

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

// TODO: add DSP algorithm functions here
float readDistanceCM() {
    // Send a 10us HIGH pulse to trigger the sensor
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read the duration of the ECHO pin being HIGH
    unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout after 30ms

    // Calculate distance in cm
    if (duration == 0) {
        return -1.0f; // indicate invalid reading
    }
    float distanceCm = duration / 58; // Speed of sound is ~343 m/s or 0.0343 cm/us
    return distanceCm;
}

float movingAverage(float newValue) {
    maBuffer[maIndex] = newValue;
    maIndex++;

    if (maIndex >= MA_WINDOW) {
    maIndex = 0;
    maFilled = true;
    }

    int count = maFilled ? MA_WINDOW : maIndex;
    if (count == 0) return newValue;

    float sum = 0;
    for (int i = 0; i < count; i++) {
    sum += maBuffer[i];
    }
    return sum / count;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    // TODO: add codes for handling your sensor setup (pinMode, etc.)
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW);

    // TODO: name your device to avoid conflictions
    BLEDevice::init("XIAO_ESP32S3_Eric_Server");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setValue("Hello World");
    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
    // TODO: add codes for handling your sensor readings (analogRead, etc.)
    rawDistanceCm = readDistanceCM();

    // TODO: use your defined DSP algorithm to process the readings
    if (rawDistanceCm >= 0.0f) {
        denoisedDistanceCm = movingAverage(rawDistanceCm);
    } else {
        denoisedDistanceCm = -1.0f; // invalid reading
    }

    Serial.print("Raw (cm): ");
    Serial.print(rawDistanceCm, 2);
    Serial.print("   |   Denoised (cm): ");
    Serial.println(denoisedDistanceCm, 2);
    
    if (deviceConnected && denoisedDistanceCm < 30.0f && denoisedDistanceCm >= 0.0f) {
        // Send new readings to database
        // TODO: change the following code to send your own readings and processed data
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        char notifyString[64];
        snprintf(notifyString, sizeof(notifyString), "%.2f", denoisedDistanceCm);
        pCharacteristic->setValue(notifyString);
        pCharacteristic->notify();
        Serial.print("Notified value: ");
        Serial.println(notifyString);
        }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising();  // advertise again
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
    delay(1000);
}
