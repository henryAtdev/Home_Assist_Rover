#include "motor.h"
#include <Arduino.h> 

Motor::Motor(int new_pin_motor_links, int new_pin_motor_rechts){
    this->pin_motor_links = new_pin_motor_links;
    this->pin_motor_rechts = new_pin_motor_rechts;
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

void Motor::winkelFahren(int Winkel){

}