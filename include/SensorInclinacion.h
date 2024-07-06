class SensorInclinacion {
public:
    SensorInclinacion(int pin) : pin(pin) { // Inicializar con el pin de entrada
        pinMode(pin, INPUT);
    }
    
    // void update() {
    //     bool stepping = false;
    //     if (!stepping && analogRead(pin) > 512) { // Asumimos que el sensor está inclinado si el valor es mayor que la mitad
    //         steps++;
    //         stepping = true;
    //     } else if (analogRead(pin) <= 512) {
    //         stepping = false;
    //     }
    //     // int sensorValue = analogRead(pin);
    //     // if (sensorValue > 512) { // Asumimos que el sensor está inclinado si el valor es mayor que la mitad
    //     //     steps++;
    //     // }
    // }
    
    // int getSteps() {
    //     return steps;
    // }
    
    // bool isSleeping() {
    //     int sensorValue = analogRead(pin);
    //     return sensorValue < 100; // Asumimos que está "dormido" si el valor es muy bajo
    // }
    
    // void resetSteps() {
    //     steps = 0;
    // }

    bool isInclined() {
        return analogRead(pin) > 512; // Asumimos que el sensor está inclinado si el valor es mayor que la mitad
    }
    
private:
    int pin;
    // int steps;
};