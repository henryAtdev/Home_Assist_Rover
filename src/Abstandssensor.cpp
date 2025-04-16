#include "Abstandssensor.h"
#include <Arduino.h>

Abstandssensor::Abstandssensor(int genauigkeit){
    _genauigkeit = genauigkeit;
    pinMode(39, INPUT);
}

void Abstandssensor::update(){
    double calcAbs = 0;
    for(int i = 0; i<_genauigkeit; ++i){
        int actVal = analogRead(39);
        calcAbs+=actVal;
    }
    calcAbs = calcAbs/_genauigkeit;
    _abstand = 3369.2 * pow(calcAbs, -0.818);
}
int Abstandssensor::getAbstand(){
    //Serial.println(_abstand);
    return _abstand;
}

