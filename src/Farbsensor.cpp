#include "Farbsensor.h"
#include "Arduino.h"

Farbsensor* Farbsensor::instance = nullptr;

Farbsensor::Farbsensor(int farbsensorInputPin, int farbsensorOutputPin){

    _ledOutputSensor = farbsensorOutputPin;
    _ledInputSensor = farbsensorInputPin;

    pinMode(_ledInputSensor, OUTPUT);
    pinMode(_ledOutputSensor, OUTPUT);

    digitalWrite(_ledOutputSensor, HIGH);
}


int Farbsensor::getActColour(){
    int actColour = analogRead(_ledInputSensor);
    return actColour;
}

Farbsensor* Farbsensor::getinstance(int farbsensorInputPin, int farbsensorOutputPin){
    if(instance == nullptr){
        instance = new Farbsensor(farbsensorInputPin, farbsensorOutputPin);
    }
    return instance;
}