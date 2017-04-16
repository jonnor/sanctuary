#include "sanctuary.hpp"

#include <CapacitiveSensor.h>

Config config;
struct IO { 
    const int sendPin = 11;
    const int receivePin = 10;
    const int samples = 10;
    CapacitiveSensor sensor = CapacitiveSensor(sendPin, receivePin);
    const int ledPin = 13;
};

IO io;

void setup() {
  io.sensor.set_CS_Timeout_Millis(10);
  pinMode(io.ledPin, OUTPUT);
  Serial.begin(115200);
}

Input readInputs() {
    return Input {
        capacitance: io.sensor.capacitiveSensor(io.samples),
        time: millis(),    
    };
}

void loop() {
  const long beforeRead = millis();
  const Input input = readInputs();
  const long afterRead = millis();

  Serial.print("capacitance: ");
  Serial.println(input.capacitance);

  const bool isOn = (input.capacitance > 100);
  if (isOn) {
    Serial.println("leds on!");
  }
  digitalWrite(io.ledPin, isOn);
  delay(100); // FIXME: remove
}
