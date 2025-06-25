/*
#include <Arduino.h>
#define LED_BUILTIN 2
#define DELAY 500

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(DELAY);
  digitalWrite(LED_BUILTIN, LOW);
  delay(DELAY);
}

*/
/*
#include <Arduino.h>

#define LED_BUILTIN 2
#define DELAY 1000 //ms


void setup() {
Serial.begin(115200); // s'utilitza per inicialitzar la comunicació sèrie entre l'Arduino i un altre dispositiu.
pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
    
digitalWrite(LED_BUILTIN, HIGH);

Serial.println("ON"); 
delay(DELAY);
digitalWrite(LED_BUILTIN, LOW);

Serial.println("OFF"); 
delay(DELAY);
}
*/

/*

#include <Arduino.h>

#define LED_BUILTIN_PIN 2
#define DELAY 1000 // ms

// Establecer un puntero al registro de salida GPIO
volatile uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;

void setup() {
    Serial.begin(115200);  // Inicializa la comunicación serie
    pinMode(LED_BUILTIN_PIN, OUTPUT); // Configura el pin como salida
}

void loop() {
    // Encender el LED activando el bit correspondiente
    *gpio_out |= (1 << LED_BUILTIN_PIN);
    Serial.println("ON");
    delay(DELAY);

    // Apagar el LED alternando el estado del bit correspondiente
    *gpio_out ^= (1 << LED_BUILTIN_PIN);
    Serial.println("OFF");
    delay(DELAY);
}
*//*


#include <Arduino.h>

#define TEST_PIN 2  // Cambia este número a un pin disponible
volatile uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;

void setup() {
    Serial.begin(115200);
    pinMode(TEST_PIN, OUTPUT);
}
void loop() {
    // ===== CASO 1: Con Serial y funciones de Arduino =====
    digitalWrite(TEST_PIN, HIGH);
    Serial.println("ON");
    digitalWrite(TEST_PIN, LOW);
    Serial.println("OFF");

}

#include <Arduino.h>

#define TEST_PIN 2  // Cambia este número a un pin disponible
volatile uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;

void setup() {
    Serial.begin(115200);
    pinMode(TEST_PIN, OUTPUT);
}
void loop() {
        // ===== CASO 2: Con Serial y acceso a registros =====
    *gpio_out |= (1 << TEST_PIN);
    Serial.println("ON");
    *gpio_out ^= (1 << TEST_PIN);
    Serial.println("OFF");

}

#include <Arduino.h>

#define TEST_PIN 2 // Cambia este número a un pin disponible
volatile uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;

void setup() {
    Serial.begin(115200);
    pinMode(TEST_PIN, OUTPUT);
}

void loop() {
    // ===== CASO 3: Sin Serial y funciones de Arduino =====
    digitalWrite(TEST_PIN, HIGH);
    digitalWrite(TEST_PIN, LOW);
}

#include <Arduino.h>

#define TEST_PIN 2 // Cambia este número a un pin disponible
volatile uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;

void setup() {
    Serial.begin(115200);
    pinMode(TEST_PIN, OUTPUT);
}

void loop() {
    // ===== CASO 4: Sin Serial y acceso a registros =====
    *gpio_out |= (1 << TEST_PIN);
    *gpio_out ^= (1 << TEST_PIN);
}



#include <Arduino.h>

#define ESP32_TEMP_SENSOR
void setup() {
    Serial.begin(115200);  // Inicializa la comunicación serie
}

void loop() {
    // Lee el valor del sensor de temperatura interna del ESP32
    int rawTemp = analogRead(ESP32_TEMP_SENSOR);  // Lee el valor del sensor interno

    // Convierte la lectura a un valor en grados Celsius
    float temperature = (rawTemp - 32) * (3.3 / 4095.0);  // Convierte el valor ADC a voltaje
    temperature = (temperature - 0.5) * 100;  // Convierte el voltaje a temperatura en grados Celsius

    // Envía el valor de temperatura por el puerto serie
    Serial.print("Temperatura interna: ");
    Serial.print(temperature);
    Serial.println(" °C");

    delay(1000);  // Pausa de 1 segundo antes de la siguiente lectura
}
   */ 
  #include <Adafruit_NeoPixel.h>
  #include <Arduino.h>
  
  // Definir el pin y número de NeoPixels
  #define PIN            6
  #define NUMPIXELS     16
  #define DELAYVAL      500  // Tiempo (en milisegundos) para la pausa entre pixeles
  
  // Inicializar el objeto de la librería NeoPixel
  Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
  
  // Definir el rango de temperaturas (ajustar según la temperatura máxima que deseas medir)
  #define MIN_TEMP 0    // Temperatura mínima en °C (en este caso, 25°C)
  #define MAX_TEMP 40    // Temperatura máxima en °C (en este caso, 40°C)
  
  void setup() {
    Serial.begin(115200);  // Inicializa la comunicación serie
    pixels.begin();        // Inicializa el anillo de NeoPixel
    analogReadResolution(12);  // Configura la resolución del ADC a 12 bits (0-4095)
  }
  
  void loop() {
    // Lee el valor del sensor de temperatura interno
    int rawValue = analogRead(0);  // Lee el valor del sensor de temperatura interno
    float voltage = (rawValue / 4095.0) * 3.3;  // Convierte el valor ADC a voltaje
  
    // Fórmula para convertir el voltaje a temperatura (en °C)
    float temperature = (voltage - 0.5) * 100;  // Convierte el voltaje a temperatura en grados Celsius
  
    // Comprobar que la temperatura esté en un rango razonable
    if (temperature < -40 || temperature > 125) {
      temperature = 25;  // Si la temperatura está fuera de un rango razonable, asigna un valor por defecto
    }
    
    // Mostrar la temperatura en el monitor serie
    Serial.print("Temperatura interna: ");
    Serial.print(temperature);
    Serial.println(" °C");
  
    // Calcular el color basado en la temperatura (mapeamos la temperatura entre MIN_TEMP y MAX_TEMP)
    int colorValue = map(temperature, MIN_TEMP, MAX_TEMP, 0, 255);  // Mapea la temperatura a un valor entre 0 y 255
    
    // Si la temperatura está por debajo del rango, se establece a azul
    if (colorValue < 0) {
      colorValue = 0;
    }
  
    // Si la temperatura excede el rango máximo, se establece a rojo
    if (colorValue > 255) {
      colorValue = 255;
    }
  
    // Asignar colores a cada pixel (verde a rojo, dependiendo de la temperatura)
    for(int i = 0; i < NUMPIXELS; i++) {
      // Mapea el color entre verde (frío) y rojo (caliente)
      int r = colorValue;    // Color Rojo
      int g = 255 - colorValue; // Color Verde (invertido)
      int b = 0;              // Azul (no utilizado en este caso)
  
      pixels.setPixelColor(i, pixels.Color(r, g, b));  // Ajustar color del pixel
    }
    
    pixels.show();  // Mostrar la actualización en los NeoPixels
    delay(DELAYVAL);  // Pausa de 500ms antes de la siguiente actualización
  }
  