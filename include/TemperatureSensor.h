class TemperatureSensor {
public:
    TemperatureSensor(int pin) : pin(pin) {} // Inicializar con el pin de entrada
    
    float readTemperature() {
        int rawValue = analogRead(pin); // Leer valor analógico
        float voltage = (rawValue / 1024.0) * 5.0; // Convertir a voltaje
        return (voltage - 0.5) * 100; // Convertir a temperatura en grados Celsius (ecuacion para LM35)
    }
    
    // bool isFaulty(float tempRef) {
    //     float temp = readTemperature() - tempRef; 
    //     return temp < -10 || temp > 50; // Rango arbitrario, ajustar umbral
    // }

private:
    int pin;
};