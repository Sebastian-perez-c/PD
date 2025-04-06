## Sebastian Pérez

# Código Práctica 4

### EJERCICIO 1 SERIAL PRINT

```c
#include <Arduino.h>
#include <FreeRTOS.h>


void anotherTask(void * parameter) {
  for(;;) {
    Serial.println("this is another Task");
    delay(1000);
  }
  vTaskDelete(NULL);  // Nunca se ejecutará porque el bucle es infinito
}


void setup() {
  Serial.begin(115200);


  // Crear una nueva tarea
  xTaskCreate(
    anotherTask,  // Función de la tarea
    "another Task",  // Nombre de la tarea (para depuración)
    10000,  // Tamaño de la pila en bytes
    NULL,  // Parámetro de la tarea
    2,  // Prioridad de la tarea
    NULL  // Manejador de la tarea (opcional)
  );
}


void loop() {


  Serial.println("this is ESP32 Task");
  delay(1000);
}
``` 

### EJERCICIO 2 (ALTERNAN LEDS)
```c
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


#define LED1 2  // Primer LED (GPIO2)
#define LED2 20 // Segundo LED (GPIO4)


SemaphoreHandle_t semaforoLED1;  // Semáforo para controlar LED1
SemaphoreHandle_t semaforoLED2;  // Semáforo para controlar LED2


void tareaEncenderLED1(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(semaforoLED1, portMAX_DELAY)) {  // Espera el semáforo de LED1
            digitalWrite(LED1, HIGH);  // Encender LED1
            digitalWrite(LED2, LOW);   // Asegurar que LED2 esté apagado
            Serial.println("LED1 Encendido");
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Espera 1 segundo
            xSemaphoreGive(semaforoLED2);  // Libera el semáforo para LED2
        }
    }
}


void tareaEncenderLED2(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(semaforoLED2, portMAX_DELAY)) {  // Espera el semáforo de LED2
            digitalWrite(LED2, HIGH);  // Encender LED2
            digitalWrite(LED1, LOW);   // Asegurar que LED1 esté apagado
            Serial.println("LED2 Encendido");
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Espera 1 segundo
            xSemaphoreGive(semaforoLED1);  // Libera el semáforo para LED1
        }
    }
}


void setup() {
Serial.begin(115200);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);


    semaforoLED1 = xSemaphoreCreateBinary();  // Crear semáforo para LED1
    semaforoLED2 = xSemaphoreCreateBinary();  // Crear semáforo para LED2


    xTaskCreate(tareaEncenderLED1, "Encender LED1", 1000, NULL, 1, NULL);
    xTaskCreate(tareaEncenderLED2, "Encender LED2", 1000, NULL, 2, NULL);


    xSemaphoreGive(semaforoLED1);  // Iniciar el semáforo de LED1 para que sea el primero en encenderse
}


void loop() {
    // El loop está vacío porque todo se maneja con tareas en FreeRTOS
}

```
### EJERCICIO PROPUESTO EN CLASE. (SEMÁFORO)

```c
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


#define LED_PIN 2  // Pin del LED


SemaphoreHandle_t semaforoEncender;
SemaphoreHandle_t semaforoApagar;


void tareaEncenderLED(void * parameter) {
  for(;;) {
    if (xSemaphoreTake(semaforoEncender, portMAX_DELAY)) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED Encendido");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      xSemaphoreGive(semaforoApagar);  // Permitir que la otra tarea se ejecute
}
  }
}


void tareaApagarLED(void * parameter) {
  for(;;) {
    if (xSemaphoreTake(semaforoApagar, portMAX_DELAY)) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED Apagado");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      xSemaphoreGive(semaforoEncender);  // Permitir que la otra tarea se ejecute
    }
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);


  semaforoEncender = xSemaphoreCreateBinary();
  semaforoApagar = xSemaphoreCreateBinary();


  xTaskCreate(tareaEncenderLED, "Encender LED", 1000, NULL, 1, NULL);
  xTaskCreate(tareaApagarLED, "Apagar LED", 1000, NULL, 1, NULL);


  xSemaphoreGive(semaforoEncender);  // Iniciar la alternancia
}


void loop() {
  // El loop está vacío porque todo se maneja con tareas en FreeRTOS
}
```