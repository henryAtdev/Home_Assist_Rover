#ifndef GYROSKOP_H_
#define GYROSKOP_H_
#include<inttypes.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class Gyroskop{
    public:
        static Gyroskop* getInstance();
        Adafruit_MPU6050 _gyrosensor;
        void update();
        void calibrate(double);
        void setZeroAngle();
        void setAngleFactor();
        float getZGyroAngle();

    private:
        Gyroskop();
        static Gyroskop *instance;

        float _gyroX = 0;
        float _gyroY = 0;
        float _gyroZ = 0;
        
        double _offsetGyroX = 0;
        double _offsetGyroY = 0;
        double _offsetGyroZ = 0; //-0.0038710251990040941194592960528098046779632568359375;
        
        double _angleGyroOffsetX = 0;
        double _angleGyroOffsetY = 0;
        double _angleGyroOffsetZ = 0;

        double _angleCalcFactorZ = 4; //TODO: Momentan noch flasch und nicht kalibrierbar
        
        // gibt den aktuellen Wert nicht in Grad zurück sondern noch vor der Umrechnung. 
        float getXGyro();
        float getYGyro();
        float getZGyro();
};


#endif