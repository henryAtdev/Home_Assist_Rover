#pragma once

// Abstract base class "SensorActor" (Sensor Actor)
class SensorActor {
public:
    // Constructor for SensorActor.
    // Takes in a stateTopic (the topic for the sensor state) and a sendData function pointer 
    // that is responsible for sending data to a specified topic.
    // Konstruktor für SensorActor.
    // Nimmt einen stateTopic (das Thema für den Sensorzustand) und einen sendData Funktionszeiger,
    // der dafür verantwortlich ist, Daten an ein angegebenes Thema zu senden.
    SensorActor(char* stateTopic, void (*sendData)(char*, char*)) : stateTopic(stateTopic), sendData(sendData) {}

    // Virtual Destructor to ensure proper cleanup when derived classes are destroyed.
    // Virtueller Destruktor, um eine ordnungsgemäße Bereinigung zu gewährleisten, 
    // wenn abgeleitete Klassen zerstört werden.
    ~SensorActor() {
        delete stateTopic;  // Deletes the dynamically allocated memory for the state topic.
    }

    // Getter method to retrieve the stateTopic.
    // Getter-Methode zum Abrufen des stateTopic.
    char* getStateTopic() {
        return stateTopic;
    }

    // Virtual method to be overridden by derived classes to handle received data.
    // This method is empty here but will be implemented in subclasses.
    // Virtuelle Methode, die von abgeleiteten Klassen überschrieben wird, um empfangene Daten zu verarbeiten.
    virtual void recivedData();

protected:
    // Protected method to send data using the sendData function pointer.
    // This method sends data to the specified topic.
    // Geschützte Methode, um Daten unter Verwendung des sendData Funktionszeigers zu senden.
    // Diese Methode sendet Daten an das angegebene Thema.
    void send(char* data) {
        sendData(stateTopic, data);  // Calls the sendData function with stateTopic and the provided data.
    }

private:
    char* stateTopic;  // A string representing the topic associated with the sensor state.
    void (*sendData)(char*, char*);  // Function pointer for sending data to a topic.
};

// Derived class "NumericalIntegerSensor" (Numerical Integer Sensor)
class NumericalIntegerSensor : public SensorActor {
public:
    // Constructor for NumericalIntegerSensor.
    // Inherits from SensorActor and initializes the stateTopic and sendData function.
    // Konstruktor für NumericalIntegerSensor.
    // Erbt von SensorActor und initialisiert den stateTopic und den sendData Funktionszeiger.
    NumericalIntegerSensor(char* stateTopic, void (*sendData)(char*, char*)) : SensorActor(stateTopic, sendData) {}

    // Overridden method for handling received data.
    // Currently empty as no specific action is defined for received data.
    // Überschreibene Methode zur Verarbeitung empfangener Daten.
    // Derzeit leer, da keine spezifische Aktion für empfangene Daten definiert ist.
    void recivedData() override {
        // Implement the logic for handling received data here if needed.
    }

    // Method to set the sensor's value and send it as a string to the specified topic.
    // Sets an integer value and converts it into a string format to be sent to the state topic.
    // Methode zum Setzen des Werts des Sensors und zum Senden als Zeichenkette an das angegebene Thema.
    // Setzt einen ganzzahligen Wert und wandelt ihn in ein Zeichenkettenformat um, 
    // um an das stateTopic gesendet zu werden.
    void setValue(int value) {
        this->value = value;  // Sets the sensor's value.
        char buffer[11];  // Buffer to hold the string representation of the integer.
        sprintf(buffer, "%d", this->value);  // Converts the integer value to a string.
        send(buffer);  // Sends the value as a string.
    }

    // Getter method to retrieve the current integer value of the sensor.
    // Getter-Methode zum Abrufen des aktuellen ganzzahligen Werts des Sensors.
    int getValue() {
        return value;
    }

private:
    int value;  // The integer value that the sensor holds.
    // Der ganzzahlige Wert, den der Sensor speichert.
};

// Derived class "NumericalDoubleSensor" (Numerical Double Sensor)
class NumericalDoubleSensor : public SensorActor {
public:
    // Constructor for NumericalDoubleSensor.
    // Inherits from SensorActor and initializes the stateTopic and sendData function.
    // Konstruktor für NumericalDoubleSensor.
    // Erbt von SensorActor und initialisiert den stateTopic und den sendData Funktionszeiger.
    NumericalDoubleSensor(char* stateTopic, void (*sendData)(char*, char*)) : SensorActor(stateTopic, sendData) {}

    // Overridden method for handling received data.
    // Currently empty as no specific action is defined for received data.
    // Überschreibene Methode zur Verarbeitung empfangener Daten.
    // Derzeit leer, da keine spezifische Aktion für empfangene Daten definiert ist.
    void recivedData() override {
        // Implement the logic for handling received data here if needed.
    }

    // Method to set the sensor's value and send it as a string to the specified topic.
    // Sets a double value and converts it into a string format to be sent to the state topic.
    // Methode zum Setzen des Werts des Sensors und zum Senden als Zeichenkette an das angegebene Thema.
    // Setzt einen Double-Wert und wandelt ihn in ein Zeichenkettenformat um, 
    // um an das stateTopic gesendet zu werden.
    void setValue(double value) {
        this->value = value;  // Sets the sensor's value.
        char buffer[20];  // Buffer to hold the string representation of the double.
        sprintf(buffer, "%.4f", this->value);  // Converts the double value to a string with 4 decimal places.
        send(buffer);  // Sends the value as a string.
    }

    // Getter method to retrieve the current double value of the sensor.
    // Getter-Methode zum Abrufen des aktuellen Double-Werts des Sensors.
    double getValue() {
        return value;
    }

private:
    double value;  // The double value that the sensor holds.
    // Der Double-Wert, den der Sensor speichert.
};
