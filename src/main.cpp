#include <Arduino.h>
#include "creator.h"
#include "sensors.h"
#include "motor.h"
#include "Abstandssensor.h"
#include "Gyroskop.h"
#include <string.h>

#define motor_rechts 2
#define motor_links 4
#define LED_R 16 
#define LED_B 17
#define LED_G 25


// Erstellen einer Instanz des SensorCreators
SensorCreator* creater = SensorCreator::getInstance();

// Deklaration der Sensoren
NumericalIntegerSensor* winkelsensorHomeAss;        // Numerischer Integer-Sensor
NumericalDoubleSensor* entfernungssensorHomeAss;        // Numerischer Double-Sensor
NumericalDoubleSensor* sensor3;        // Numerischer Double-Sensor

Motor* myMotor = Motor::getInstance(motor_links, motor_rechts);
Abstandssensor* myAbstandssensor = new Abstandssensor(500);
Gyroskop* myGyroskop;

void updateSensoren(void *parameter) {
  while (true) {
    myAbstandssensor->update();
    myGyroskop->update();     // dieselbe globale Instanz updaten
    delay(1);
  }
}
  
void setup() {
  Serial.begin(115200);  // Initialisierung der seriellen Kommunikation
  creater->begin();      // Initialisierung des SensorCreators
  
  // Erstellung der Sensoren mit ihren jeweiligen Parametern
  winkelsensorHomeAss = (NumericalIntegerSensor*) creater->createNumericSensor("Winkel", "Grad", false, "mdi:gauge");
  winkelsensorHomeAss->setValue(0);   // Startwert des ersten Sensors auf 0 setzen
  
  entfernungssensorHomeAss = (NumericalDoubleSensor*) creater->createNumericSensor("Abstandsensor", "cm", true, "mdi:ruler");
  entfernungssensorHomeAss->setValue(99);   // Startwert des zweiten Sensors auf 99 setzen
  
  //Koordinaten müssen ebenfalls hochgeladen werden 
  //sensor3 = (NumericalDoubleSensor*) creater->createNumericSensor("Mein toller Sensor", "s", true, "mdi:gauge");
  //sensor3->setValue(0);   // Startwert des dritten Sensors auf 0 setzen

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(23, INPUT); //Gyroskop Button

  myGyroskop = Gyroskop::getInstance();
  myGyroskop->calibrate(5);
  //myGyroskop->setAngleFactor(); --> funktioniert nicht (aktuell)
  myGyroskop->setZeroAngle();
  
xTaskCreatePinnedToCore(
    updateSensoren,         // Task-Funktion
    "Gyro Update Task",     // Name
    8192,                   // Stack-Größe
    NULL,                   // Parameter
    1,                      // Priorität
    NULL,                   // Task-Handle (optional)
    1                       // Core-ID (0 oder 1)
  );
}



bool gegenstand = false;
bool fahrZustandsaenderung = true;

long testtime = millis();

void loop() {
  double Abs = myAbstandssensor->getAbstand();
  float actGyrZ = myGyroskop->getZGyroAngle();

  //Fahren
  //Serial.println(Abs);
  // Gegenstand der im Weg des Rovers ist mittels des Abstandssensors feststellen
  if (Abs<=10.0){
    gegenstand = true;
    fahrZustandsaenderung = true; 
  }
  
  // Wenn der Gegenstand im Weg ist, Zunächst um 90° nach links drehen --> Wenn auch da ein Gegenstand ist um 180° weiterdrehen
  if (gegenstand == true && fahrZustandsaenderung == true){
    winkelsensorHomeAss->setValue(actGyrZ);
    entfernungssensorHomeAss->setValue(Abs);
    
    myMotor->winkelFahren(90, 255, myGyroskop);
 
    if(myAbstandssensor->getAbstand()<=10){
      myMotor->winkelFahren(180, 255, myGyroskop);
    }

    Abs = myAbstandssensor->getAbstand();
    if (Abs <=10.0){
      winkelsensorHomeAss->setValue(actGyrZ);
      entfernungssensorHomeAss->setValue(Abs);
      myMotor->winkelFahren(180, 255, myGyroskop);
    }
    gegenstand = false;
  }
  
  if(gegenstand == false && fahrZustandsaenderung == true){

    myMotor->geradeFahren(200, myGyroskop);
    fahrZustandsaenderung = false;
  }
  
  Serial.print("Act angle: ");
  Serial.println(myGyroskop->getZGyroAngle());

}
