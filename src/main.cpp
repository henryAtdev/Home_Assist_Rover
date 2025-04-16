#include <Arduino.h>
#include "creator.h"
#include "sensors.h"
#include "motor.h"
#include "Abstandssensor.h"

#define motor_rechts 2
#define motor_links 4
#define LED_R 16 
#define LED_B 17
#define LED_G 25


// Erstellen einer Instanz des SensorCreators
SensorCreator* creater = SensorCreator::getInstance();

// Deklaration der Sensoren
NumericalIntegerSensor* sensor;        // Numerischer Integer-Sensor
NumericalDoubleSensor* sensor2;        // Numerischer Double-Sensor
NumericalDoubleSensor* sensor3;        // Numerischer Double-Sensor

Motor* myMotor = Motor::getInstance(motor_links, motor_rechts);
Abstandssensor* myAbstandssensor = new Abstandssensor(500);

void setup() {
  Serial.begin(115200);  // Initialisierung der seriellen Kommunikation
  creater->begin();      // Initialisierung des SensorCreators
  
  // Erstellung der Sensoren mit ihren jeweiligen Parametern
  sensor = (NumericalIntegerSensor*) creater->createNumericSensor("Test Counter", "count");
  sensor->setValue(0);   // Startwert des ersten Sensors auf 0 setzen
  
  sensor2 = (NumericalDoubleSensor*) creater->createNumericSensor("Double Counter", "m", true, "mdi:ruler");
  sensor2->setValue(0.0001);   // Startwert des zweiten Sensors auf 0.0001 setzen
  
  sensor3 = (NumericalDoubleSensor*) creater->createNumericSensor("Mein toller Sensor", "s", true, "mdi:gauge");
  sensor3->setValue(0);   // Startwert des dritten Sensors auf 0 setzen
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  

  int wertAbstandssensor;

  
}

void loop() {
  /*creater->loop();  // Aufrufen der loop-Funktion des SensorCreators für interne Updates

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

  //myMotor->geradeFahren(255);
  //delay(500);
  //Serial.println(wertAbstandssensor)

  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
  
  myAbstandssensor->update();
  
  double Abs = myAbstandssensor->getAbstand();
  Serial.println(Abs);
  
  
  
}
