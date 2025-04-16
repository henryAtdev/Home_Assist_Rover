#ifndef GYROSKOP_H_
#define GYROSKOP_H_
#include<inttypes.h>
class Gyroskop{
    public:
        Gyroskop();
        void update();
        int16_t getZGyro();
        unsigned long durchlaeufe;
    private:
        int16_t xBeschl;
        int16_t	yBeschl;
        int16_t zBeschl;
        int16_t xGyro;
        int16_t yGyro;
        long zGyro;
        long long zGyroTemp;
        int16_t Temperatur;
};


#endif