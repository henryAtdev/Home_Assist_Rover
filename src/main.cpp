#include <Arduino.h>
#include "creator.h"
#include "sensors.h"

#define motor_rechts 2
#define motor_links 4


// Erstellen einer Instanz des SensorCreators
SensorCreator* creater = SensorCreator::getInstance();

// Deklaration der Sensoren
NumericalIntegerSensor* sensor;        // Numerischer Integer-Sensor
NumericalDoubleSensor* sensor2;        // Numerischer Double-Sensor
NumericalDoubleSensor* sensor3;        // Numerischer Double-Sensor

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


pinMode(motor_links, OUTPUT);
pinMode(motor_rechts, OUTPUT);
}

void loop() {
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
  sensor3->setValue(millis() / 1000.0);

  analogWrite(motor_links, 255);
  analogWrite(motor_rechts, 100);

}
