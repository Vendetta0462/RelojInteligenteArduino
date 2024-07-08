#include <Arduino.h>

// Definir el pin del LED
const int ledPin = 11;

uint8_t ocho;
uint16_t dieciseis;
uint32_t treintaydos;
uint64_t sesentaycuatro;

uint16_t Low;
uint16_t High;

int brightness = 50;
// Variable para controlar la dirección de cambio del brillo
int fadeAmount = 5;


void setup() {
  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // Definir el ciclo de trabajo inicial del PWM
  analogWrite(ledPin, brightness);
  // sesentaycuatro = 500;
}

void loop() {
  // Low += 1;
  // if (Low >= 1000){
  //   Low = 0;
  // }
  // High = 1000-Low;
  // // Encender el LED
  // digitalWrite(ledPin, HIGH);
  // delayMicroseconds(High);
  // // Esperar un segundo
  // // delay(1);

  // // Apagar el LED
  // digitalWrite(ledPin, LOW);
  // delayMicroseconds(Low);
  // sesentaycuatro *= 2000;
  // Esperar un segundo
  delay(30);
}

// put function declarations here:
// int myFunction(int, int);

// union Data{
//   uint8_t entero;
//   char caracter;
// };

// struct DataStruct{
//   uint8_t entero;
//   char caracter;
//   };

// char suma;

// Data myData;
// DataStruct myDataStruct;

// void setup() {
//   // put your setup code here, to run once:
//   // int result = myFunction(2, 3);
//   Serial.begin(9600);
//   myData.entero = 65; // El valor ASCII de 'A'
//   myData.caracter = 'B'; 
//   Serial.print("Valor uint8_t union: ");
//   Serial.println(myData.entero);
//   Serial.print("Valor char union: ");
//   Serial.println(myData.caracter); // Debería imprimir 'A'
//   Serial.print("Puntero de entero union: ");
//   Serial.println((uint16_t)&myData.entero);
//   Serial.print("Puntero de caracter union: ");
//   Serial.println((uint16_t)&myData.caracter);
//   Serial.println(" ");
//   myDataStruct.entero = 65; // El valor ASCII de 'A'
//   myDataStruct.caracter = 'B';
//   Serial.print("Valor uint8_t struct: ");
//   Serial.println(myDataStruct.entero);
//   Serial.print("Valor char struct: ");
//   Serial.println(myDataStruct.caracter);
//   Serial.print("Puntero de entero struct: ");
//   Serial.println((uint16_t)&myDataStruct.entero);
//   Serial.print("Puntero de caracter struct: ");
//   Serial.println((uint16_t)&myDataStruct.caracter);
// }

// void loop() {
//   // char suma;
//   // put your main code here, to run repeatedly:
//   // Serial.println(suma);
//   // Serial.println((uint16_t)&suma);
//   // suma = 78;
//   // delay(500);
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }