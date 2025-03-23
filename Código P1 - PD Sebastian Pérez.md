# <center>Informe Práctica 1</center>

## Apartado 2.


```
#define LED_BUILTIN 2  // Pin del LED (cambia si usas otro)
#define DELAY 1000     // Tiempo de espera en milisegundos

void setup() { 
  pinMode(LED_BUILTIN, OUTPUT);  // Iniciar el pin del LED como salida
  Serial.begin(115200);          // Iniciar el puerto serie
} 
 
void loop() { 
  digitalWrite(LED_BUILTIN, HIGH); // Encender LED
  Serial.println("ON");            // Enviar mensaje al puerto serie
  delay(DELAY);                    // Esperar 1000 ms

  digitalWrite(LED_BUILTIN, LOW);  // Apagar LED
  Serial.println("OFF");           // Enviar mensaje al puerto serie
  delay(DELAY);                    // Esperar 1000 ms
}

```
## Apartado 3.
```
#include <Arduino.h>
#include "soc/gpio_reg.h"


#define LED_PIN 2
#define DELAY 1000


void setup() {
  Serial.begin(115200);
 
  // Configura el pin 2 (LED_BUILTIN) como salida
  pinMode(LED_PIN, OUTPUT);
}


void loop() {
  // Encender el LED (activar el pin)
  GPIO.out_w1ts = (1 << LED_PIN); // Set the output to high (turn on)
  Serial.println("ON");
  delay(DELAY);


  // Apagar el LED (desactivar el pin)
  GPIO.out_w1tc = (1 << LED_PIN); // Set the output to low (turn off)
  Serial.println("OFF");
  delay(DELAY);
}
``` 
## Apartado 4.

### Apartado 4.1
```
#include <Arduino.h>
#include "soc/gpio_reg.h"




#define LED_PIN 4  // Definir el pin de salida (puedes cambiarlo)


void setup() {
  Serial.begin(115200);


    pinMode(LED_PIN, OUTPUT); // Configurar el pin como salida
}


void loop() {
    digitalWrite(LED_PIN, HIGH); // Encender
    Serial.println("ON");
    digitalWrite(LED_PIN, LOW);  // Apagar
     Serial.println("OFF");
   
}
```
### Aparatdo 4.2
```
// SIN ARDUINO CON ENVIO 

#include <Arduino.h>
#include "soc/gpio_reg.h"

#define LED_PIN 4


void setup() {
  Serial.begin(115200);
 
  // Configura el pin 4 (LED_BUILTIN) como salida
  pinMode(LED_PIN, OUTPUT);
}


void loop() {
  // Encender el LED (activar el pin)
  GPIO.out_w1ts = (1 << LED_PIN); // Set the output to high (turn on)
  Serial.println("ON");


  // Apagar el LED (desactivar el pin)
  GPIO.out_w1tc = (1 << LED_PIN); // Set the output to low (turn off)
  Serial.println("OFF");
}
```
### Apartado 4.3

```
// ARDUINO SIN SERIAL PRINT

#define LED_PIN 4  // Definir el pin de salida (puedes cambiarlo)

void setup() {
    pinMode(LED_PIN, OUTPUT); // Configurar el pin como salida
}

void loop() {
    digitalWrite(LED_PIN, HIGH); // Encender
    
    digitalWrite(LED_PIN, LOW);  // Apagar
   
}

```
### Apartado 4.4
```
#include <Arduino.h>
#include "soc/gpio_reg.h"  // Librería para acceder a los registros GPIO


#define PIN_SALIDA 4  // Cambia este número por el pin que desees usar


void setup() {
    // Configurar el pin como salida
    uint32_t *gpio_enable = (uint32_t *)GPIO_ENABLE_REG;
    *gpio_enable |= (1 << PIN_SALIDA);
}


void loop() {
    uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;


    // Alternar el estado del pin a máxima velocidad
    *gpio_out ^= (1 << PIN_SALIDA);
}
```

## Apartado 6.
```
#include <Arduino.h>
#include "soc/gpio_reg.h"
#define DELAY
#define PIN_SALIDA 4  


void setup() {
    Serial.begin(115200);


    // Configurar el pin como salida
    uint32_t *gpio_enable = (uint32_t *)GPIO_ENABLE_REG;
    *gpio_enable |= (1 << PIN_SALIDA);
}


void loop() {
    uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;


    unsigned long t1 = micros(); // Capturar tiempo inicial


    *gpio_out |= (1 << PIN_SALIDA);  // Encender LED
    *gpio_out &= ~(1 << PIN_SALIDA); // Apagar LED


    unsigned long t2 = micros(); // Capturar tiempo final


    Serial.print("Tiempo de ejecución: ");
    Serial.print(t2 - t1); // Mostrar tiempo en microsegundos
    Serial.println(" us");


    delay(1000); // Esperar 1 segundo antes de la siguiente medición
}

``` 

## Ejercicio para subir nota.
```
#include <Arduino.h>

#define PIN_ADC 34  // Pin de entrada A/D
#define PIN_DAC 25  // Pin de salida D/A

void setup() {
    Serial.begin(115200);  // Iniciar comunicación serie
    pinMode(PIN_ADC, INPUT); // Configurar el pin de entrada A/D
}

void loop() {
    // Leer el valor analógico (0-4095) del ADC
    int valorADC = analogRead(PIN_ADC);
    
    // Escalar el valor de 0-4095 a 0-255 para el DAC
    int valorDAC = map(valorADC, 0, 4095, 0, 255);
    
    // Enviar el valor al puerto serie
    Serial.print("ADC: ");
    Serial.print(valorADC);
    Serial.print(" -> DAC: ");
    Serial.println(valorDAC);

    // Escribir el valor en la salida D/A
    dacWrite(PIN_DAC, valorDAC);
    
    delay(100); // Pequeña pausa para estabilidad
}
```
