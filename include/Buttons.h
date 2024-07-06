class Buttons {
public:
    Buttons(int pin) : pin(pin) {}  // Inicializar con el pin de entrada
    
    bool changePressed(){
        return readPin() > 900; // Asumimos que el botón está presionado si el valor es mayor que cierto umbral
    }

    bool resetPressed(){
        return readPin() < 90; // Asumimos que el botón está presionado si el valor es menor que cierto umbral
    }

private:
    int pin;

    int readPin() {
        return analogRead(pin);
    }
};