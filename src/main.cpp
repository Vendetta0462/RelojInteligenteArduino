// #include <Wire.h>
// #include <Adafruit_LiquidCrystal.h>
#include "Display.h"
#include "LightSensor.h"
#include "TemperatureSensor.h"
#include "Vibrator.h"
#include "Buzzer.h"
#include "SensorInclinacion.h"
#include "Buttons.h"


class SmartWatch {
public:
    SmartWatch() :  // Inicializar con los distintos componentes y variables
        lightSensor(A3),
        tempSensor(A1),
        inclinacionSensor(A2),
        vibrator(2),
        buzzer(3),
        button(A0),
        caloriesBurned(0),
        timeAwake(0),
        isDaytime(true),
        lastUpdateTime(0),
        displayMode(0),
        isFaulty(false),
        tempRef(0),
        eventTriggered(false)
    {}
    
    bool eventTriggered;

    void init() { 
        display.init();
        // pinMode(buttonPin, INPUT);
    }

    void check() {
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime > 200) { // Actualizar cada cierto tiempo
            lastUpdateTime = currentTime;
            
            // inclinacionSensor.update(); // Actualizar el sensor de inclinación
            // checkTemperature(); // Comprobar la temperatura
            // checkCalories(); // Comprobar las calorías
            // checkSleep(); // Comprobar si está durmiendo
            // checkDayNight(); // Comprobar si es de día o de noche
        
            // Cambio de temperatura
            checkTemperature();
            // Fallo en la temperatura
            // checkTempFailure();
            // Cambio en los pasos
            checkSteps();
            // Cambio en el estado de sueño
            checkSleep();
            // Cambio de día a noche
            // checkDayNight();
            // Cambio de noche a día
            // checkNightDay();
            // Cambio de luminacion
            checkLight();
            // Cambio en boton
            checkButton();

        }
    }
    
    void update() { // Actualizar el reloj


        updateDisplay(); // Actualizar la pantalla
        
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
    Buttons button;
    
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
    watch.check();
    if (watch.eventTriggered){
        watch.update();
        watch.eventTriggered = false;
    }
}