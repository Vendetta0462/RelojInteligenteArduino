class Buzzer {
public:
    Buzzer(int pin) : pin(pin) { // Inicializar con el pin de salida
        pinMode(pin, OUTPUT);
    }
    
    void beep(int duration) { // Emitir una serie de 3 pitidos cortos con duración total de "duration"
		tone(pin, 500, duration / 5);
        delay(duration / 5);
        tone(pin, 500, duration / 5);
        delay(duration / 5);
        tone(pin, 500, duration / 5);
    }

private:
    int pin;
};