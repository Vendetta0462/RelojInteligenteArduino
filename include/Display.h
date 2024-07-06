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