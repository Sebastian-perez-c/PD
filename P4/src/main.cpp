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



