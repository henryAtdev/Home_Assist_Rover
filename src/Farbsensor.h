#ifndef FARBSENSOR_
#define FARBSENSOR_

#include <inttypes.h>
#include <Arduino.h>

typedef struct{
    int _ledInputSensorLinks;
    int _ledInputSensorRechts;
}Farbsensorwerte;


class Farbsensor{
    private:
        int _ledInputSensorLinks;
        int _ledInputSensorRechts;
        static Farbsensor* instance;
        Farbsensor(int farbsensorInputPin, int farbSensorOutputPin);
        Farbsensorwerte _farbsensorwerte;

    public:        
        Farbsensorwerte* getActColour();
        static Farbsensor* getinstance(int farbsensorInputPin, int farbsensorOutputPin);
        bool isBlack();
};

#endif