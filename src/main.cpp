#include <Arduino.h>
#include "creator.h"
#include "sensors.h"
#include "motor.h"
#include "Abstandssensor.h"
#include "Gyroskop.h"
#include "Farbsensor.h"
#include <string.h>

#define motor_rechts 2
#define motor_links 4
#define LED_R 16 
#define LED_B 17
#define LED_G 25

#define updatePause 10 //des Winkelsensors
#define updatePauseAbstandssensor 100

enum Fahrtzustand{
  gegenstandVorraus,
  geradeaus,
  zwischenfahrtLinks,
  zwischenfahrtRechts,
  isDriving
};
enum lastDirection{
  links,
  rechts
};


// Erstellen einer Instanz des SensorCreators
SensorCreator* creater = SensorCreator::getInstance();

// Deklaration der Sensoren
NumericalIntegerSensor* winkelsensorHomeAss;        // Numerischer Integer-Sensor
NumericalDoubleSensor* entfernungssensorHomeAss;        // Numerischer Double-Sensor
NumericalDoubleSensor* sensor3;        // Numerischer Double-Sensor

Motor* myMotor = Motor::getInstance(motor_links, motor_rechts);
Abstandssensor* myAbstandssensor = new Abstandssensor(500);
Gyroskop* myGyroskop;
Farbsensor *myFarbsensor;

long starttime = millis();

void updateAbstandssensor(void *parameter){
  TickType_t lastWakeTime = xTaskGetTickCount(); // Startzeit
  const TickType_t frequency = updatePauseAbstandssensor / portTICK_PERIOD_MS;  // Angabe von updatePause gibt an wie oft geupdated wird
  while (true) {
    TickType_t now = xTaskGetTickCount();
    if(now - lastWakeTime>frequency){
      Serial.println("Update von Abstandssensor wird zu langsam Ausgeführt");
    }
    myAbstandssensor->update();                 // Abstandssensor abfragen
    myMotor->updateGeradeausFahren(myGyroskop);
    vTaskDelayUntil(&lastWakeTime, frequency);
  }
}

void updateSensoren(void *parameter) {
  TickType_t lastWakeTime = xTaskGetTickCount(); // Startzeit
  const TickType_t frequency = updatePause / portTICK_PERIOD_MS;  // Angabe von updatePause gibt an wie oft geupdated wird

  while (true) {
    TickType_t now = xTaskGetTickCount();
    if(now - lastWakeTime>frequency){
      Serial.println("Update wird zu langsam Ausgeführt");
    }
    myGyroskop->update();                       // Gyro abfragen                // Abstandssensor abfragen
    vTaskDelayUntil(&lastWakeTime, frequency);  // exakt die dauer von updatePause warten
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

  myFarbsensor = Farbsensor::getinstance(34, 36);
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
xTaskCreatePinnedToCore(
    updateAbstandssensor,             // Task-Funktion
    "Abstandssensor Update Task",     // Name
    8192,                             // Stack-Größe
    NULL,                             // Parameter
    1,                                // Priorität
    NULL,                             // Task-Handle (optional)
    0                                 // Core-ID (0 oder 1)
  );
  bool isBlack =true;
  unsigned long timeOnBlack;
  unsigned long timeBevorBlack = millis();
  myMotor->gesteuertesGeradeFahren(200, myGyroskop);
  while(isBlack){
    isBlack=myFarbsensor->isBlack();
  }
  timeOnBlack=millis();
  int timeOverAll= timeOnBlack - timeBevorBlack;
  Serial.println(timeOverAll);
}

long testtime = millis();
Fahrtzustand actFahrzustand = geradeaus; //initialisierung des Zustandes
lastDirection lastDrovenDirection = rechts;




void loop() {
  double Abs = myAbstandssensor->getAbstand();
  float actGyrZ = myGyroskop->getZGyroAngle();


  //Zustandsautomat für den Algorithmus zum abfahren des Raumes; TODO: zurücksetzen auf geradeFahren
  if (Abs<=12.0){
    actFahrzustand = gegenstandVorraus;
  }
  if (actFahrzustand == gegenstandVorraus && lastDrovenDirection == rechts){
    actFahrzustand = zwischenfahrtLinks;
    lastDrovenDirection = links;
  }
  else if(actFahrzustand == gegenstandVorraus && lastDrovenDirection == links){
    actFahrzustand = zwischenfahrtRechts;
    lastDrovenDirection = rechts;
  }

  //Auszuführende Bedingungen im jeweiligen Zustand
  if (actFahrzustand == geradeaus){
    myMotor->gesteuertesGeradeFahren(200, myGyroskop);
    actFahrzustand = isDriving;
  }
  else if(actFahrzustand == zwischenfahrtLinks){
    //SENDE DATEN
    myMotor->winkelFahren(90, 200, myGyroskop);
    Abs = myAbstandssensor->getAbstand();
    if(Abs>18){
      myMotor->streckeFahren(2000, myGyroskop);
      myMotor->winkelFahren(-90, 200, myGyroskop);
      Abs = myAbstandssensor->getAbstand();
      if(Abs < 13.0){
        myMotor->winkelFahren(-180, 200, myGyroskop);
      }
    }
    else{
      myMotor->winkelFahren(90, 200, myGyroskop);
    }
    actFahrzustand = geradeaus;
  }
  else if(actFahrzustand == zwischenfahrtRechts){
    //SENDE DATEN
    myMotor->winkelFahren(-90, 200, myGyroskop);
    Abs = myAbstandssensor->getAbstand();
    if(Abs>18){
      myMotor->streckeFahren(2000, myGyroskop);
      myMotor->winkelFahren(90, 200, myGyroskop);
      Abs = myAbstandssensor->getAbstand();
      if(Abs < 13.0){
        myMotor->winkelFahren(180, 200, myGyroskop);
      }
    }
    else{
      myMotor->winkelFahren(-90, 200, myGyroskop);
    }
    actFahrzustand = geradeaus;
  }


}
