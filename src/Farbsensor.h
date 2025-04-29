#ifndef FARBSENSOR_
#define FARBSENSOR_

#include <inttypes.h>
#include <Arduino.h>



class Farbsensor{
    private:
        int _ledOutputSensor;
        int _ledInputSensor;
        static Farbsensor* instance;
        Farbsensor(int farbsensorInputPin, int farbSensorOutputPin);

    public:        
        int getActColour();
        static Farbsensor* getinstance(int farbsensorInputPin, int farbsensorOutputPin);
};

#endif