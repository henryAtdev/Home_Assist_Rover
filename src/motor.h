#ifndef MOTOR_H
#define MOTOR_H

#include "Gyroskop.h"

class Motor{
    public:
        
        void streckeFahren(int fahrzeit);
        void winkelFahren(int winkel, int speed, Gyroskop* myGyr);
        void kurveFahren(int power_links , int power_rechts);
        void motorAus();
        static Motor* getInstance(int new_pin_motor_links, int new_pin_motor_rechts);
        bool autonomfahren();
        void updateWinkelFahren(Gyroskop* myGyr);
        void geradeFahren(int speed, Gyroskop*);

    private:
        int modus;
        void update();
        int power_motor_links;
        int power_motor_rechts;
        int pin_motor_links;
        int pin_motor_rechts;
        Motor(int new_pin_motor_links, int new_pin_motor_rechts);
        static Motor* instance;
        void geradeFahren(int geschwindigkeit);
        int zuFahrenderWinkel;
        

};

#endif