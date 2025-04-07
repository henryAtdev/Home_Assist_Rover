#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "sensors.h"

// Singleton class to manage the creation of sensors and communication via MQTT
// Singleton-Klasse zur Verwaltung der Erstellung von Sensoren und der Kommunikation über MQTT
class SensorCreator
{
public:
    // Public static method to get the singleton instance of the SensorCreator
    // Öffentliche statische Methode, um die Singleton-Instanz des SensorCreator zu erhalten
    static SensorCreator* getInstance();

    // Destructor for cleaning up resources.
    // Destruktor zur Bereinigung von Ressourcen
    ~SensorCreator();

    // Main loop method that runs repeatedly.
    // Hauptloop-Methode, die wiederholt ausgeführt wird.
    void loop();

    // Initializes the sensor creator with WiFi credentials.
    // If no credentials are provided, defaults to nullptr.
    // Initialisiert den SensorCreator mit WiFi-Anmeldeinformationen.
    // Wenn keine Anmeldeinformationen angegeben sind, wird standardmäßig nullptr verwendet.
    void begin(char* ssid = nullptr, char* passwd = nullptr);

    // Creates a numeric sensor (either integer or floating-point).
    // Takes the sensor name, unit, whether it's a floating point sensor, and an optional icon.
    // Erstellt einen numerischen Sensor (entweder ganzzahlig oder Gleitkommazahl).
    // Nimmt den Sensornamen, die Einheit, ob es sich um einen Gleitkommasensor handelt, und ein optionales Symbol entgegen.
    SensorActor* createNumericSensor(char* sensorName, char* unit, bool isFloatingPoint = false, char* icon = "mdi:counter");

    // Sends data to a specified topic via MQTT.
    // Sendet Daten an ein angegebenes Thema über MQTT.
    void sendData(char* topic, char* data);

private:
    PubSubClient* client;  // MQTT client instance used for communication
    // MQTT-Client-Instanz, die für die Kommunikation verwendet wird
    char name[25] = "Rover-x";  // Name of the device (default: "Rover-x")
    char device[250] = "";  // Device identifier, potentially for a specific device
    WiFiClient wifi;  // WiFi client for network communication
    // WiFi-Client für die Netzwerkkommunikation

    static SensorCreator* instance;  // Static instance of the SensorCreator (for singleton pattern)
    // Statische Instanz des SensorCreator (für das Singleton-Muster)

    // Private constructor to ensure the class can only be instantiated once.
    // Privater Konstruktor, um sicherzustellen, dass die Klasse nur einmal instanziiert wird.
    SensorCreator();

    // Deleted copy constructor and assignment operator to prevent copying.
    // Gelöschter Kopierkonstruktor und Zuweisungsoperator, um Kopieren zu verhindern.
    SensorCreator(const SensorCreator&) = delete;
    SensorCreator& operator=(const SensorCreator&) = delete;

    // Private method to establish a connection to WiFi and MQTT broker.
    // Private Methode zum Herstellen einer Verbindung zum WiFi und MQTT-Broker.
    void connect();
};
