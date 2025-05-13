#include "motor.h"
#include <Arduino.h> 

#include "Gyroskop.h"

Motor* Motor::instance = nullptr; 

Motor::Motor(int new_pin_motor_links, int new_pin_motor_rechts){
    this->pin_motor_links = new_pin_motor_links;
    this->pin_motor_rechts = new_pin_motor_rechts;
    pinMode(pin_motor_rechts, OUTPUT);
    pinMode(pin_motor_links, OUTPUT);
}

Motor* Motor::getInstance(int new_pin_motor_links, int new_pin_motor_rechts){
    if (instance == nullptr){
        instance = new Motor(new_pin_motor_links, new_pin_motor_rechts);
    }
    return instance;
}

void Motor::motorAus(){
    this->power_motor_links = 0;
    this->power_motor_rechts = 0;
    update();
}

void Motor::geradeFahren(int geschwindigkeit){
    this->power_motor_links = geschwindigkeit;
    this->power_motor_rechts = geschwindigkeit;
    update();
}

void Motor::update(){
    analogWrite(this->pin_motor_links, this->power_motor_links);
    analogWrite(this->pin_motor_rechts, this->power_motor_rechts);
}

void Motor::kurveFahren(int power_links , int power_rechts){
    this->power_motor_links = power_links;
    this->power_motor_rechts = power_rechts;
    update();
}

void Motor::winkelFahren(int winkel, int speed, Gyroskop* myGyr){
    lastDirection = false;
    double anfangsWinkel = myGyr->getZGyroAngle();
    double updatedWinkel = anfangsWinkel;
    if (winkel>0){
            kurveFahren(0, speed);
            while(winkel + anfangsWinkel > updatedWinkel){
                updatedWinkel = myGyr->getZGyroAngle();
                Serial.print("Während: ");  
                Serial.println(updatedWinkel);  
            }
        }
    if (winkel<0){
        kurveFahren(speed, 0);
        while(anfangsWinkel + winkel < updatedWinkel){
            updatedWinkel = myGyr->getZGyroAngle();
            Serial.print("Während: ");  
            Serial.println(updatedWinkel);  
        }
        }    
    motorAus();
    
}

void Motor::updateWinkelFahren(Gyroskop* myGyr){
    float actAngle = myGyr->getZGyroAngle();
    if(actAngle > zuFahrenderWinkel){
        winkelFahren(0,0, myGyr);
        zuFahrenderWinkel = 0;
    }
}

void Motor::updateGeradeausFahren(Gyroskop* myGyr){
    if(lastDirection == true){
        Serial.println("Hier");
        double angleError =  geradeAusfahrregelung_.startangle - myGyr->getZGyroAngle();
        
        // Regelung mittels PID
        float P = geradeAusfahrregelung_.Kp * angleError;
        double D = angleError - geradeAusfahrregelung_.letzterKorrekturfaktor;
        
        geradeAusfahrregelung_.fehlerSumme += angleError;
        double I = geradeAusfahrregelung_.Ki * geradeAusfahrregelung_.fehlerSumme;

        int correction = P + D +I;
        this->power_motor_links = constrain(200 - correction, 0, 255);
        this->power_motor_rechts = constrain(200 + correction, 0, 255);

        update();
    }
}

void Motor::streckeFahren(int fahrzeit, Gyroskop* myGyr){
    unsigned long startzeit = millis();
    while(startzeit + fahrzeit <= millis()){
        this->gesteuertesGeradeFahren(200, myGyr);
    }
}

void Motor::geradeFahren(int geschwindigkeit, Gyroskop* myGyr){
    this->power_motor_links = geschwindigkeit;
    this->power_motor_rechts = geschwindigkeit;
    update();
}

void Motor::gesteuertesGeradeFahren(int geschwindigkeit, Gyroskop* myGyr){
    if(lastDirection == false){
        lastDirection = true;
        geradeAusfahrregelung_.startangle = myGyr->getZGyroAngle();
        this->power_motor_links = geschwindigkeit;
        this->power_motor_rechts = geschwindigkeit;
        geradeAusfahrregelung_.Anfangsgeschwindigkeit = geschwindigkeit;
        update();
    }
}