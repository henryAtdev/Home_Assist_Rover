#include "Abstandssensor.h"
#include <Arduino.h>
#define ABSTANDSSENSORPIN 39

portMUX_TYPE absMux = portMUX_INITIALIZER_UNLOCKED;

Abstandssensor::Abstandssensor(int genauigkeit){
    _genauigkeit = genauigkeit;
    pinMode(ABSTANDSSENSORPIN, INPUT);
}

void Abstandssensor::update(){
    double calcAbs = 0;
    for(int i = 0; i<_genauigkeit; ++i){
        int actVal = analogRead(39);
        calcAbs+=actVal;
    }
    calcAbs = calcAbs/_genauigkeit;
    portENTER_CRITICAL(&absMux);
    _abstand = 3369.2 * pow(calcAbs, -0.818);
    portEXIT_CRITICAL(&absMux);
}

int Abstandssensor::getAbstand(){
    portENTER_CRITICAL(&absMux);
    double achtAbstand = _abstand;
    portEXIT_CRITICAL(&absMux);
    return _abstand;
}

