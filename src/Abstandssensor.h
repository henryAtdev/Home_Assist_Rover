#ifndef ABSTANDSSENSOR_ 
#define ABSTANDSSENSOR_

class Abstandssensor{       //TODO: Abfrage ob über 39cm! --> danach sehr sehr hohe Ungenauigkeit
    private:
        double _abstand;
        int _genauigkeit;
    public:
        Abstandssensor(int genauigkeit);
        void update();
        int getAbstand();
};


#endif