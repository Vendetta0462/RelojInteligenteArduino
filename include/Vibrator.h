class Vibrator {
public:
    Vibrator(int pin) : pin(pin) { // Inicializar con el pin de salida
        pinMode(pin, OUTPUT);
    }
    
    void vibrate(int duration) {
        digitalWrite(pin, HIGH);
        delay(duration);
        digitalWrite(pin, LOW);
    }

private:
    int pin;
};