class LightSensor {
public:
    LightSensor(int pin) : pin(pin) {}  // Inicializar con el pin de entrada
    
    int readLight() {
        return analogRead(pin);
    }

private:
    int pin;
};