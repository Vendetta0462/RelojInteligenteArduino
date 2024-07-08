#include <Sensors_Actuators.h> // Incluye las clases de sensores y actuadores

// Clases para la lógica de características específicas

class TemperatureFaliureDetector {
public:
    void update(float temperature, float tempRef) { // Actualizar con la temperatura actual y la de referencia
        isFaulty = ((temperature - tempRef) < -10 || (temperature - tempRef) > 50); // Falla si la temperatura está fuera de rango
        }

        bool getIsFaulty() const {
        return isFaulty;
        }
private:
    bool isFaulty;
};

class StepCounter {
public:
    void update(bool isInclined) {
        if (isInclined && !wasInclined) { // Si se inclinó y no estaba inclinado antes
            steps++; // Contar un paso
        }
        wasInclined = isInclined; 
    }

    int getSteps() const {
        return steps;
    }

    void reset() {
        steps = 0;
    }

private:
    int steps = 0;
    bool wasInclined = false;
};

class SleepDetector {
public:
    void update(bool isInclined) {
        if (isInclined) { // Si está inclinado
            if (timeInclined == 0) { // Si no estaba inclinado antes
                timeInclined = millis(); // Guardar el tiempo actual
            }
        } else { // Si no está inclinado
            timeInclined = 0; // Reiniciar el tiempo
        }
    }

    bool isSleeping() const {
        return timeInclined > 0 && millis() - timeInclined > 5000; // Duerme si ha pasado más de x segundos inclinado
    }

private:
    unsigned long timeInclined = 0;
};

class CalorieCalculator {
public:
    void update(int steps) {
        caloriesBurned = steps * 0.04; // Calorías quemadas por paso (asumiendo una persona de 70kg que quema 80 calorías por milla caminado)
    }

    int getCaloriesBurned() const {
        return caloriesBurned;
    }

private:
    int caloriesBurned = 0;
};

class DayNightDetector {
public:
    DayNightDetector(Buzzer& buzzer, Vibrator& vibrator, StepCounter& stepCounter)
        : buzzer(buzzer), vibrator(vibrator), stepCounter(stepCounter) {} // Inicializar con los actuadores y el contador de pasos

    void update(int lightLevel) {
        bool newIsDaytime = lightLevel > 500; // Determinar si es de día o de noche
        if (newIsDaytime != isDaytime) { // Si cambió el estado
            isDaytime = newIsDaytime;
            if (isDaytime) { // Si es de día
                onDawn(); // Realizar acciones al amanecer
            }
        }
    }

    bool getIsDaytime() const {
        return isDaytime;
    }

private:
    bool isDaytime = false;
    Buzzer& buzzer;
    Vibrator& vibrator;
    StepCounter& stepCounter;

    void onDawn() { // Acciones al amanecer
        buzzer.activate(500);
        vibrator.activate(1000);
        stepCounter.reset();
    }
};