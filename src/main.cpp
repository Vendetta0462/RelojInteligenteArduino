#include "Sensors_Actuators.h" // Incluye las clases de sensores y actuadores
#include "Logic.h"          // Incluye las clases de lógica

// Clase principal SmartWatch
class SmartWatch {
public:
    SmartWatch() :
        lightSensor(A3),
        tempSensor(A1),
        inclinationSensor(A2),
        vibrator(2),
        buzzer(3),
        buttonPin(A0),
        timeAwake(0),
        lastUpdateTime(0),
        displayMode(0),
        tempRef(0),
        dayNightDetector(buzzer, vibrator, stepCounter)
    {}
    
    void init() {
        display.init();
        pinMode(buttonPin, INPUT);
        tempRef = 0;
    }
    
    void update() {
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime > 200) { // Actualizar cada x tiempo
            lastUpdateTime = currentTime;
            
            updateSensors(); // Actualizar los sensores
            updateLogic(); // Actualizar la lógica
            updateDisplay(); // Actualizar la pantalla
        }
        
        handleButton(); // Manejar los botones
    }

private:
    Display display;
    LightSensor lightSensor;
    TemperatureSensor tempSensor;
    InclinationSensor inclinationSensor;
    Vibrator vibrator;
    Buzzer buzzer;
    
    StepCounter stepCounter;
    SleepDetector sleepDetector;
    CalorieCalculator calorieCalculator;
    DayNightDetector dayNightDetector;
    
    int buttonPin;
    unsigned long timeAwake;
    unsigned long lastUpdateTime;
    int displayMode;
    float tempRef;
    
    void updateSensors() {
        lightSensor.update();
        tempSensor.update();
        inclinationSensor.update();
    }
    
    void updateLogic() {
        stepCounter.update(inclinationSensor.getIsInclined());
        sleepDetector.update(inclinationSensor.getIsInclined());
        calorieCalculator.update(stepCounter.getSteps());
        dayNightDetector.update(lightSensor.getLight());
        
        if (sleepDetector.isSleeping()) { // Si está durmiendo
                timeAwake = 0; // Reiniciar el tiempo despierto
        } else { // Si está despierto
            timeAwake += 1; // Incrementar el tiempo despierto en 1 segundo
        }
        
        if (calorieCalculator.getCaloriesBurned() > 500) { // Si ha quemado más de 500 calorías
            vibrator.activate(500);
        }
        
        if (tempSensor.getIsFaulty()) { // Si el sensor de temperatura falla
            display.showInfo("Temp Sensor Error"); // Mostrar mensaje de error
            buzzer.activate(500);
            vibrator.activate(1000);
        }
    }
    
    void updateDisplay() {
        display.setBacklight(lightSensor.getLight()); // Ajustar el brillo de la pantalla
        
        if (!tempSensor.getIsFaulty()) { // Si el sensor de temperatura no falla
            String info;
            switch (displayMode) {
                case 0:
                    info = "Temp: " + String(tempSensor.getTemperature() - tempRef) + "C";
                    break;
                case 1:
                    info = "Steps: " + String(stepCounter.getSteps());
                    break;
                case 2:
                    info = "Cal: " + String(calorieCalculator.getCaloriesBurned());
                    break;
                case 3:
                    info = "Awake: " + String(timeAwake / 3600) + "h " + String((timeAwake % 3600) / 60) + "m " + String(timeAwake % 60) + "s";
                    break;
            }
            display.showInfo(info); // Mostrar la información en la pantalla
        }
    }
    
    void handleButton() {
        int buttonValue = analogRead(buttonPin);
        if (buttonValue > 900) { // Si se presiona el botón de informacion (umbral alto)
            displayMode = (displayMode + 1) % 4; // Cambiar el modo de visualización
            updateDisplay(); // Actualizar la pantalla
            delay(400); // Esperar para evitar múltiples pulsaciones
        } else if (buttonValue < 100) { // Si se presiona el botón de reset (umbral bajo)
            reset(); // Resetear el reloj
            delay(400);
        }
    }
    
    void reset() {
        stepCounter.reset(); // Resetear los pasos
        timeAwake = 0; // Resetear el tiempo despierto
        display.showInfo("System Reset"); // Mostrar mensaje de reset
        delay(2000);
        tempRef = tempSensor.getTemperature() - 24; // Guardar la temperatura actual como referencia "ambiente"
    }
};

SmartWatch watch;

void setup() {
    watch.init();
}

void loop() {
    watch.update();
}