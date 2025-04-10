#ifndef MOTOR_H
#define MOTOR_H

class Motor{
    public:
        void geradeFahren(int geschwindigkeit);
        void winkelFahren(int winkel);         //noch nicht implementiert, evt. noch implementieren wenn sensoren da sind
        void kurveFahren(int power_links , int power_rechts);
        void motorAus();
        static Motor* getInstance(int new_pin_motor_links, int new_pin_motor_rechts);
        
    private:
        void update();
        int power_motor_links;
        int power_motor_rechts;
        int pin_motor_links;
        int pin_motor_rechts;
        Motor(int new_pin_motor_links, int new_pin_motor_rechts);
        static Motor* instance;

};

#endif