#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

// Initializing ultrasonic sensor ids
#define FRONT_TRIG -1
#define FRONT_ECHO -1

#define INNER_TRIG 5
#define INNER_ECHO 16

// Initializing LED and Servo ids
#define NEOPIXEL_PIN 25
#define SERVO -1

const float EMPTY_DISTANCE = 30.0f;
const float FULL_DISTANCE = 5.0f;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800); // 16 LEDs

void setup()
{
    Serial.begin(115200);
    pinMode(INNER_TRIG, OUTPUT);
    pinMode(INNER_ECHO, INPUT);
    digitalWrite(INNER_TRIG, LOW);

    ring.begin();
    ring.setBrightness();
    ring.clear();
    ring.show();
}

// read distance sensor, return in units of centimeters
float readDistance(int trig, int echo)
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10); // 10us pulse to start sensing
    digitalWrite(trig, LOW);

    unsigned long timeout = micros() + 26233L; // timeout to prevent infinite loop
    while ((digitalRead(echo) == LOW) && (micros() < timeout))
        ;
    unsigned long start_time = micros();
    timeout = start_time + 26233L;
    while ((digitalRead(echo) == HIGH) && (micros() < timeout))
        ;
    unsigned long lapse = micros() - start_time;
    return lapse * 0.01716f;
}

int getCapacity()
{
    float distance = readDistance(INNER_TRIG, INNER_ECHO);

    if (distance < 0)
        return -1;

    float percentage = (EMPTY_DISTANCE - distance) / (EMPTY_DISTANCE - FULL_DISTANCE) * 100.0f;

    percentage = constrain(percentage, 0.0f, 100.0f);

    return static_cast<int>(percentage);
}

void loop()
{
    int capacity = getCapacity();

    Serial.print("Trash Capacity: ");
    Serial.print(capacity);
    Serial.println("%");

    delay(500);
}
