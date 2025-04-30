#include "Gyroskop.h"
#include <Wire.h>
#include<Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Gyroskop* Gyroskop::instance = nullptr;

Gyroskop::Gyroskop(){
    _gyrosensor.begin();
    
    // Festlegen der Genauigkeit des Sensors (von 250Deg bis 2000) --> je kleiner desto genauer
    _gyrosensor.setGyroRange(MPU6050_RANGE_250_DEG);
}

Gyroskop* Gyroskop::getInstance(){
    if(instance == nullptr){
        instance =  new Gyroskop();
    }
    return instance;
}

// Misst und speichert den aktuellen Wert des Sensors. Muss um eine hohe Genauigkeit zu haben möglichst oft ausgeführt werden.
void Gyroskop::update(){
    sensors_event_t accelEvent;
    sensors_event_t gyroEvent;
    sensors_event_t tempEvent;
    
    _gyrosensor.getEvent(&accelEvent, &gyroEvent, &tempEvent);

    _gyroX += gyroEvent.gyro.x - _offsetGyroX;
    _gyroY += gyroEvent.gyro.y - _offsetGyroY;
    _gyroZ += gyroEvent.gyro.z - _offsetGyroZ;
}

float Gyroskop::getXGyro(){
    return _gyroX;
}

float Gyroskop::getYGyro(){
    return _gyroY;
}

float Gyroskop::getZGyro(){
    double calcAbsAngle = (_gyroZ - _angleGyroOffsetZ);
    return calcAbsAngle;
}

float Gyroskop::getZGyroAngle(){
    return getZGyro()/_angleCalcFactorZ;
}

/* Funktion calibrate:
// Diese Funktion dient dazu den Offset festzustellen. Dieser wird am Ende in der Variablen _Ofset gespeichert. 
// Der Übergabeparameter der Dauer legt fest wie lange die Kalibrierung dauer --> Je länger desto genauer.
// Während der Kalibrierung darf der Sensor nicht bewegt werden.  
// TODO: Bisher nur für das Gyroskop implementiert. 
*/

void Gyroskop::calibrate(double dauerInSek){
    Serial.print("Starte Kalibrierung, Dauer = ");
    Serial.print(dauerInSek);
    Serial.println(" Sekunden");

    _offsetGyroX = 0; 
    _offsetGyroY = 0;
    _offsetGyroZ = 0;

    unsigned long anfangsZeitpunkt = millis();
    unsigned long durchlaeufe = 0;

    float lastX = getXGyro();
    float lastY = getYGyro();
    float lastZ = getZGyro();
    
    double totalDiffGyroX = 0;
    double totalDiffGyroY = 0;
    double totalDiffGyroZ = 0;

    int countSec = 0;

    while(anfangsZeitpunkt + dauerInSek*1000>millis()){
        update();
        float actX = getXGyro();
        float actY = getYGyro();
        float actZ = getZGyro();

        float diffX = actX - lastX;
        float diffY = actY - lastY;
        float diffZ = actZ - lastZ;

        totalDiffGyroX += diffX;
        totalDiffGyroY += diffY;
        totalDiffGyroZ += diffZ;
        
        lastX = actX;
        lastY = actY;
        lastZ = actZ;

        ++durchlaeufe;
        if(anfangsZeitpunkt+1000*countSec>millis()){
            ++countSec;
            Serial.print("Kalibriert seit ");
            Serial.print(countSec);
            Serial.println(" Sekunden");
        }
    }

    _offsetGyroX = totalDiffGyroX/durchlaeufe;
    _offsetGyroY = totalDiffGyroY/durchlaeufe;
    _offsetGyroZ = totalDiffGyroZ/durchlaeufe;

    /*Serial.println("Kalibrierung abgeschlossen");
    Serial.print("Offset X * 1000: ");
    Serial.print(1000*_offsetGyroX);
    Serial.print("Offset Y * 1000: ");
    Serial.print(1000*_offsetGyroY);
    Serial.print("Offset Z * 1000: ");
    Serial.print(_offsetGyroZ, 100); 
    Serial.println();*/
}

// definiert den aktuellen Winkel als 0°
void Gyroskop::setZeroAngle(){
    _angleGyroOffsetZ = 0;

    _angleGyroOffsetZ = getZGyro();
}

// TODO: Ziel: Sensor wird einmal im Kreis gedreht und daraus kann berechnet werden wie hoch der Faktor zwischen Winkel und ausgegebenen Werten ist.

void Gyroskop::setAngleFactor(){

    double zeroPoint = getZGyroAngle();
    double zeroPoint360 = 0;
    unsigned long actTime = millis();
    int a = digitalRead(23);
    Serial.println("Knopf drücken wenn Kalibrierung beendet ist.");
    double passthrough = 0;
    while(a == 0){
        ++passthrough;
        update();
        zeroPoint360 += getZGyro(); 
        a = digitalRead(23); 
    }
    zeroPoint360 = zeroPoint360/passthrough;
    _angleCalcFactorZ = zeroPoint360/360;
    Serial.println("Kalibrierung 360° beendet");
    Serial.println(_angleCalcFactorZ);
}

