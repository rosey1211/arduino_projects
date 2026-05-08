#include <myClass.h>
#include <Arduino.h>
#include <pumpController.h>

myClass classInstance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  classInstance.setInternalValue(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  classInstance.printInternalValue();
}
