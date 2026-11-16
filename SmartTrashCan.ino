#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

// Initializing ultrasonic sensor ids
#define FRONT_TRIG = 5
#define FRONT_ECHO = 16

#define INNER_TRIG = -1
#define INNER_ECHO = -1

// Initializing LED and Servo ids
#define NEOPIXEL_PIN = -1
#define SERVO = 23

#define DISTANCE_THRESHOLD = 30

Servo servo;

void setup()
{
    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);

    servo.attach(SERVO);
    servo.write(0);
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

void loop()
{
    float distance = readDistance();

    if (distance > 0 && distance < DISTANCE_THRESHOLD)
    {
        servo.write(90);
    }
    else
    {
        servo.write(0);
    }
}
