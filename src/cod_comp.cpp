#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Clases para los distintos componentes del reloj
class Display {
public:
    Display() : lcd(0) {} // Inicializar con dirección I2C 0
    
    void init() {
        lcd.begin(16, 2); // Inicializar con 16 columnas y 2 filas
        lcd.setBacklight(LOW); // Encender retroiluminación
    }
    
    void setBacklight(int level) {
        lcd.setBacklight(level > 500 ? HIGH : LOW); // Encender retroiluminación si el nivel es mayor que cierto umbral
    }
    
    void showInfo(const String& info) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(info);
    }

private:
    Adafruit_LiquidCrystal lcd; // Objeto de la librería Adafruit
};

class LightSensor {
public:
    LightSensor(int pin) : pin(pin) {}  // Inicializar con el pin de entrada
    
    int readLight() {
        return analogRead(pin);
    }

private:
    int pin;
};

class TemperatureSensor {
public:
    TemperatureSensor(int pin) : pin(pin) {} // Inicializar con el pin de entrada
    
    float readTemperature() {
        int rawValue = analogRead(pin); // Leer valor analógico
        float voltage = (rawValue / 1024.0) * 5.0; // Convertir a voltaje
        return (voltage - 0.5) * 100; // Convertir a temperatura en grados Celsius (ecuacion para LM35)
    }
    
    bool isFaulty(float tempRef) {
        float temp = readTemperature() - tempRef; 
        return temp < -10 || temp > 50; // Rango arbitrario, ajustar umbral
    }

private:
    int pin;
};

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

class SensorInclinacion {
public:
    SensorInclinacion(int pin) : pin(pin), steps(0) { // Inicializar con el pin de entrada
        pinMode(pin, INPUT);
    }
    
    void update() {
        bool stepping = false;
        if (!stepping && analogRead(pin) > 512) { // Asumimos que el sensor está inclinado si el valor es mayor que la mitad
            steps++;
            stepping = true;
        } else if (analogRead(pin) <= 512) {
            stepping = false;
        }
        // int sensorValue = analogRead(pin);
        // if (sensorValue > 512) { // Asumimos que el sensor está inclinado si el valor es mayor que la mitad
        //     steps++;
        // }
    }
    
    int getSteps() {
        return steps;
    }
    
    bool isSleeping() {
        int sensorValue = analogRead(pin);
        return sensorValue < 100; // Asumimos que está "dormido" si el valor es muy bajo
    }
    
    void resetSteps() {
        steps = 0;
    }

private:
    int pin;
    int steps;
};

class SmartWatch {
public:
    SmartWatch() :  // Inicializar con los distintos componentes y variables
        lightSensor(A3),
        tempSensor(A1),
        inclinacionSensor(A2),
        vibrator(2),
        buzzer(3),
        buttonPin(A0),
        caloriesBurned(0),
        timeAwake(0),
        isDaytime(true),
        lastUpdateTime(0),
        displayMode(0),
        isFaulty(false),
        tempRef(0)
    {}
    
    void init() { 
        display.init();
        pinMode(buttonPin, INPUT);
    }
    
    void update() { // Actualizar el reloj
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime > 200) { // Actualizar cada cierto tiempo
            lastUpdateTime = currentTime;
            
            inclinacionSensor.update(); // Actualizar el sensor de inclinación
            checkTemperature(); // Comprobar la temperatura
            checkCalories(); // Comprobar las calorías
            checkSleep(); // Comprobar si está durmiendo
            checkDayNight(); // Comprobar si es de día o de noche
            updateDisplay(); // Actualizar la pantalla
        }
        
        handleButton(); // Manejar los botones
    }
    
    void handleButton() {
        int buttonValue = analogRead(buttonPin);
        if (buttonValue > 900) { // Botón de cambio de información
            displayMode = (displayMode + 1) % 4; // Circular entre 4 modos
            updateDisplay();
            delay(400); // Prevenir múltiples pulsaciones
        } else if (buttonValue < 100) { // Botón de reset
            reset();
            delay(400); // Prevenir múltiples pulsaciones
        }
    }
    
    void reset() { // Resetear el reloj
        inclinacionSensor.resetSteps(); // Resetear los pasos
        caloriesBurned = 0; // Resetear las calorías
        timeAwake = 0; // Resetear el tiempo despierto
        display.showInfo("System Reset"); // Mostrar mensaje de reset
        delay(2000); // Esperar 2 segundos
        tempRef = tempSensor.readTemperature() - 24; // Guardar la temperatura actual como referencia
    }

private: // Componentes y variables del reloj
    Display display;
    LightSensor lightSensor;
    TemperatureSensor tempSensor;
    SensorInclinacion inclinacionSensor;
    Vibrator vibrator;
    Buzzer buzzer;
    
    int buttonPin;
    int caloriesBurned;
    unsigned long timeAwake;
    bool isDaytime;
    unsigned long lastUpdateTime;
    int displayMode;
    bool isFaulty;
    float tempRef;
    
    void updateDisplay() { // Actualizar la pantalla con la información correspondiente
        int lightLevel = lightSensor.readLight(); 
        display.setBacklight(lightLevel); // Ajustar la retroiluminación según la luz ambiente
        
        String info;
        switch (displayMode) { // Mostrar información según el modo
            case 0:
                info = "Temp: " + String(tempSensor.readTemperature() - tempRef) + "C";
                break;
            case 1:
                info = "Steps: " + String(inclinacionSensor.getSteps());
                break;
            case 2:
                info = "Cal: " + String(caloriesBurned);
                break;
            case 3:
                info = "Awake: " + String(timeAwake / 3600) + "h " + String((timeAwake % 3600) / 60) + "m " + String(timeAwake % 60) + "s";
                break;
        }
        display.showInfo(info); // Mostrar la información en la pantalla
    }
    
    void checkTemperature() { // Comprobar la temperatura y mostrar un mensaje si es anormal
        if (tempSensor.isFaulty(tempRef)) {
            display.showInfo("Temp Sensor Error");
            if (!isFaulty) { // Solo vibrar y sonar si no estaba ya en estado de fallo
                buzzer.beep(500);
                vibrator.vibrate(1000);
            }
        }
        isFaulty = tempSensor.isFaulty(tempRef);
    }
    
    void checkCalories() {
        // Cálculo simple de calorías basado en pasos (ajustar modelo)
        caloriesBurned = inclinacionSensor.getSteps() * 0.05;
        
        if (caloriesBurned > 500) { // Umbral arbitrario (ajustar umbral)
            vibrator.vibrate(500);
        }
    }
    
    void checkSleep() {
        if (inclinacionSensor.isSleeping()) {
            if (isDaytime) {
                timeAwake = 0;
            }
        } else {
            timeAwake += 1; // Incrementar en segundos
        }
    }
    
    void checkDayNight() {
        int lightLevel = lightSensor.readLight(); // Leer nivel de luz
        isDaytime = lightLevel > 500; // Umbral arbitrario, ajustar umbral
        
        static bool wasDaytime = true; // Variable estática para recordar el estado anterior
        if (isDaytime && !wasDaytime) { 
            // Amanecer
            buzzer.beep(500);
            vibrator.vibrate(1000);
        }
        wasDaytime = isDaytime;
    }
};

SmartWatch watch;

void setup() {
    watch.init();
}

void loop() {
    watch.update();
}