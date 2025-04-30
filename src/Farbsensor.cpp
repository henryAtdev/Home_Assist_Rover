#include "Farbsensor.h"
#include "Arduino.h"

Farbsensor* Farbsensor::instance = nullptr;

Farbsensor::Farbsensor(int farbsensorInputPinLinks, int farbsensorInputPinRechts){

    _ledInputSensorLinks = farbsensorInputPinLinks;
    _ledInputSensorRechts = farbsensorInputPinRechts;

    pinMode(_ledInputSensorLinks, INPUT);
    pinMode(_ledInputSensorRechts, INPUT);
}


Farbsensorwerte* Farbsensor::getActColour(){
    _farbsensorwerte._ledInputSensorLinks = analogRead(_ledInputSensorLinks);
    _farbsensorwerte._ledInputSensorRechts = analogRead(_ledInputSensorRechts);
    return &_farbsensorwerte;
}

Farbsensor* Farbsensor::getinstance(int farbsensorInputPin, int farbsensorOutputPin){
    if(instance == nullptr){
        instance = new Farbsensor(farbsensorInputPin, farbsensorOutputPin);
    }
    return instance;
}