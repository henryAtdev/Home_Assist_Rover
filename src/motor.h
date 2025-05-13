#ifndef MOTOR_H
#define MOTOR_H

#include "Gyroskop.h"

class Motor{
    public:
        long lasttimeUpdatedMotor = 0;    

        void streckeFahren(int fahrzeit, Gyroskop* myGyr);
        void winkelFahren(int winkel, int speed, Gyroskop* myGyr);
        void kurveFahren(int power_links , int power_rechts);
        void motorAus();
        static Motor* getInstance(int new_pin_motor_links, int new_pin_motor_rechts);
        bool autonomfahren();
        void updateWinkelFahren(Gyroskop* myGyr);
        void updateGeradeausFahren(Gyroskop* myGyr);
        void geradeFahren(int speed, Gyroskop*);
        void gesteuertesGeradeFahren(int speed, Gyroskop*);

    private:
        static Motor* instance;
        int modus;
        int power_motor_links = 0;
        int power_motor_rechts = 0;
        int pin_motor_links;
        int pin_motor_rechts;
        
        int zuFahrenderWinkel;
        

        struct geradeausFahrregelung{
            double Kp = 0.1;
            double Ki = 0.1;
            double Kd = 0.01;
            double fehlerSumme;
            double startangle;
            double KorrekturFaktor = 2;
            int Anfangsgeschwindigkeit = 0;
            int letzterKorrekturfaktor = 0;
        } geradeAusfahrregelung_;

        bool lastDirection = false; // false da er am Anfang geradeaus fahren soll. wenn false fährt er aktuell Kurve; wenn true geradeaus
        Motor(int new_pin_motor_links, int new_pin_motor_rechts);
        void update();
        void geradeFahren(int geschwindigkeit);        

};

#endif