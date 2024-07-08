// Incluimos las librerías necesarias de Arduino
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Clases base para los componentes
class Sensor {
public:
    virtual void update() = 0; // Método virtual puro para actualizar el sensor
};

class Actuator {
public:
    virtual void activate(int duration) = 0; // Método virtual puro para activar el actuador
};

// Clases para los componentes específicos
class Display : public Actuator {
public:
    Display() : lcd(0) {} // Inicializar con dirección I2C 0
    
    void init() {
        lcd.begin(16, 2);
        lcd.setBacklight(LOW);
    }
    
    void setBacklight(int level) {
        lcd.setBacklight(level > 500 ? HIGH : LOW); // Encender retroiluminación si el nivel es mayor que cierto umbral
    }
    
    void showInfo(const String& info) {
        if (info != currentInfo) { // Solo actualizar si la información es distinta a la anterior
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(info);
            currentInfo = info;
        }
    }

    void activate(int duration) override {
        // No requerido para el display
    }

    String getCurrentInfo() const {
        return currentInfo;
    }

private:
    Adafruit_LiquidCrystal lcd; // Objeto de la librería Adafruit
    String currentInfo;
};

class LightSensor : public Sensor {
public:
    LightSensor(int pin) : pin(pin), lightLevel(0) {}
    
    void update() override {
        lightLevel = analogRead(pin); // Leer valor analógico
    }

    int getLight() const {
        return lightLevel;
    }

private:
    int pin;
    int lightLevel;
};

class TemperatureSensor : public Sensor {
public:
    TemperatureSensor(int pin) : pin(pin), temperature(0){}
    
    void update() override {
        int rawValue = analogRead(pin); // Leer valor analógico
        float voltage = (rawValue / 1024.0) * 5.0; // Convertir a voltaje
        temperature = (voltage - 0.5) * 100; // Convertir a temperatura en grados Celsius (ecuacion para TMP36)
    }

    float getTemperature() const {
        return temperature;
    }

private:
    int pin;
    float temperature;
};

class InclinationSensor : public Sensor {
public:
    InclinationSensor(int pin) : pin(pin), isInclined(false) {}
    
    void update() override {
        isInclined = analogRead(pin) > 512; // Determinar si el sensor está inclinado
    }

    bool getIsInclined() const {
        return isInclined;
    }

private:
    int pin;
    bool isInclined;
};

class Vibrator : public Actuator {
public:
    Vibrator(int pin) : pin(pin) {
        pinMode(pin, OUTPUT);
    }
    
    void activate(int duration) override {
        digitalWrite(pin, HIGH);
        delay(duration);
        digitalWrite(pin, LOW);
    }

private:
    int pin;
};

class Buzzer : public Actuator {
public:
    Buzzer(int pin) : pin(pin) {
        pinMode(pin, OUTPUT);
    }
    
    void activate(int duration) override { // Emitir una serie de 3 pitidos cortos con duración total de "duration"
        tone(pin, 500, duration / 5);
        delay(duration / 5);
        tone(pin, 500, duration / 5);
        delay(duration / 5);
        tone(pin, 500, duration / 5);
    }

private:
    int pin;
};