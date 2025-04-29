#include <Arduino.h>
#include "creator.h"
#include "sensors.h"
#include "motor.h"
#include "Abstandssensor.h"
#include "Gyroskop.h"

#define motor_rechts 2
#define motor_links 4
#define LED_R 16 
#define LED_B 17
#define LED_G 25


// Erstellen einer Instanz des SensorCreators
SensorCreator* creater = SensorCreator::getInstance();

// Deklaration der Sensoren
NumericalIntegerSensor* Winkelsensor;        // Numerischer Integer-Sensor
NumericalDoubleSensor* Entfernungssensor;        // Numerischer Double-Sensor
NumericalDoubleSensor* sensor3;        // Numerischer Double-Sensor

Motor* myMotor = Motor::getInstance(motor_links, motor_rechts);
Abstandssensor* myAbstandssensor = new Abstandssensor(500);
Gyroskop* myGyroskop;

void setup() {
  Serial.begin(115200);  // Initialisierung der seriellen Kommunikation
  /*creater->begin();      // Initialisierung des SensorCreators
  
  // Erstellung der Sensoren mit ihren jeweiligen Parametern
  Winkelsensor = (NumericalIntegerSensor*) creater->createNumericSensor("Winkel", "Grad", false, "mdi:gauge");
  Winkelsensor->setValue(0);   // Startwert des ersten Sensors auf 0 setzen
  
  Entfernungssensor = (NumericalDoubleSensor*) creater->createNumericSensor("Abstandsensor", "cm", true, "mdi:ruler");
  Entfernungssensor->setValue(99);   // Startwert des zweiten Sensors auf 99 setzen
  
  //Koordinaten müssen ebenfalls hochgeladen werden 
  //sensor3 = (NumericalDoubleSensor*) creater->createNumericSensor("Mein toller Sensor", "s", true, "mdi:gauge");
  //sensor3->setValue(0);   // Startwert des dritten Sensors auf 0 setzen
  */
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(23, INPUT); //Gyroskop Button

  myGyroskop = Gyroskop::getInstance();
  myGyroskop->calibrate(1);
  //myGyroskop->setAngleFactor();
  myGyroskop->setZeroAngle();
}

void updateSensoren(){
  myAbstandssensor->update();
  myGyroskop->update();

}

bool gegenstand = false;
bool fahrZustandsaenderung = true;

long testtime = millis();

void loop() {
  /* Beispiels überbleibsel
  
  creater->loop();  // Aufrufen der loop-Funktion des SensorCreators für interne Updates

  // Erhöhen des ersten Sensors um 1 bei jedem Durchlauf der loop-Funktion
  sensor->setValue(sensor->getValue() + 1);
  delay(250);  // Kurze Pause von 250 Millisekunden
  
  // Verdoppeln des zweiten Sensors, mit einer längeren Verzögerung
  sensor2->setValue(sensor2->getValue() * 2);
  delay(750);  // Längere Pause von 750 Millisekunden

  // Zurücksetzen des zweiten Sensors, wenn sein Wert größer als 1000 wird
  if (sensor2->getValue() > 1000) {
    sensor2->setValue(0.000005);
  }

  // Setzen des dritten Sensors auf die Anzahl der Millisekunden seit Programmstart, in Sekunden umgerechnet
  sensor3->setValue(millis() / 1000.0);*/


  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
  
 
  updateSensoren();
  double Abs = myAbstandssensor->getAbstand();
  float actGyrZ = myGyroskop->getZGyroAngle();


  //Fahren
  Serial.println(Abs);

  if (Abs<10.0){
    gegenstand = true;
    fahrZustandsaenderung = true; 
  }
  
  if (gegenstand == true && fahrZustandsaenderung == true){
    //TODO: sonsor daten hochladen
    myMotor->winkelFahren(90, 255, myGyroskop);
    //myMotor->streckeFahren(1000);
    myMotor->winkelFahren(-90, 255, myGyroskop);
    updateSensoren();
    Abs = myAbstandssensor->getAbstand();
    if (Abs <=10.0){
      //TODO: hier ebenfalls Dateien hochladen
      myMotor->winkelFahren(180, 255, myGyroskop);
    }
    gegenstand = false;
  }
  
  if(gegenstand == false && fahrZustandsaenderung == true){
    myMotor->geradeFahren(200);
    fahrZustandsaenderung = false;
    
  }


}
