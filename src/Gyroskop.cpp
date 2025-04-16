#include "Gyroskop.h"
#include <Wire.h>
#include<Arduino.h>
#define GYRO_ADDR 0x68

Gyroskop::Gyroskop(){
    durchlaeufe = 0;
    /*Serial.print("Gyroskoperstellung begonnen");
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // wake up!
    Wire.endTransmission(true);
    durchlaeufe = 0;
    Serial.println("Gyroskop erfolgrecih erstellt");
    */
}
void Gyroskop::update(){
    Wire.beginTransmission(GYRO_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. --> Nach dem Datenerhalt wird der Bus weiterhin offen gehalten
    Wire.requestFrom(GYRO_ADDR, 14, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same int16_t variable
    xBeschl = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    yBeschl = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    zBeschl = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    Temperatur = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    xGyro = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    yGyro = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    zGyro += (Wire.read()<<8 | Wire.read())/100; // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
    //zGyro = zGyro+35;
    durchlaeufe+=1;
}

int16_t Gyroskop::getZGyro(){
    int16_t	 actangle = zGyro;
    return actangle;
}