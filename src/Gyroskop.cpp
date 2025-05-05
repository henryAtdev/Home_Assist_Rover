#include "Gyroskop.h"
#include <Wire.h>
#include<Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define LED_R 16 
#define LED_B 17
#define LED_G 25

portMUX_TYPE gyroMux = portMUX_INITIALIZER_UNLOCKED;
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
    
    portENTER_CRITICAL(&gyroMux);
    _gyroX += gyroEvent.gyro.x - _offsetGyroX;
    _gyroY += gyroEvent.gyro.y - _offsetGyroY;
    _gyroZ += gyroEvent.gyro.z - _offsetGyroZ;
    portEXIT_CRITICAL(&gyroMux);
    delay(10);
}

float Gyroskop::getXGyro(){
    portENTER_CRITICAL(&gyroMux);
    float val = _gyroX;
    portEXIT_CRITICAL(&gyroMux);
    return val;
}

float Gyroskop::getYGyro(){
    portENTER_CRITICAL(&gyroMux);
    float val = _gyroY;
    portEXIT_CRITICAL(&gyroMux);
    return val;
}

float Gyroskop::getZGyro(){
    portENTER_CRITICAL(&gyroMux);
    float calcAbsAngle = (_gyroZ - _angleGyroOffsetZ);
    portEXIT_CRITICAL(&gyroMux);
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

    analogWrite(LED_R, 255);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);

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


    analogWrite(LED_R, 0);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 0);
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
    portENTER_CRITICAL(&gyroMux);
    _angleGyroOffsetZ = getZGyro();
    portEXIT_CRITICAL(&gyroMux);
}

// TODO: Ziel: Sensor wird einmal im Kreis gedreht und daraus kann berechnet werden wie hoch der Faktor zwischen Winkel und ausgegebenen Werten ist.

void Gyroskop::setAngleFactor(){

    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 255);
    double zeroPoint = getZGyroAngle();
    double zeroPoint360 = 0;
    unsigned long actTime = millis();
    int a = digitalRead(23);
    Serial.println("Knopf drücken wenn Kalibrierung beendet ist.");
    
    double passthrough = 0;
    double lastGoThrough = 0;
    
    while(a == 0){
        
        if(lastGoThrough == getZGyro()){    
            ++passthrough;
            lastGoThrough = getZGyro();
            zeroPoint360 += lastGoThrough;
        }     
        a = digitalRead(23); 
        //update();
        
    }
    zeroPoint360 = zeroPoint360/passthrough;
    _angleCalcFactorZ = zeroPoint360/360;
    Serial.println("Kalibrierung 360° beendet");
    Serial.println(_angleCalcFactorZ);
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 0);
}

