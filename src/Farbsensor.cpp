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

Farbsensor* Farbsensor::getinstance(int farbsensorInputPinLinks, int farbsensorOutputPin){
    if(instance == nullptr){
        instance = new Farbsensor(farbsensorInputPinLinks, farbsensorOutputPin);
    }
    return instance;
}
bool Farbsensor::isBlack(){
    int linksFarbsensor_ = 0;
    int bb = 0;
    int durchlaeufe = 0;
    for(int i=0; i<=60; ++i){
        durchlaeufe =  durchlaeufe +1;
        linksFarbsensor_ += analogRead(_ledInputSensorLinks);
        bb += analogRead(34);
    }
    linksFarbsensor_ = linksFarbsensor_/durchlaeufe;
    bb = bb/durchlaeufe;

    if (linksFarbsensor_<20){
        return false;
    }
    if (linksFarbsensor_>=20){
        return true;
    }
}