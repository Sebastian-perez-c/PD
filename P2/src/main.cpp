#include <Arduino.h>

#include <Arduino.h>

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

const int LED_PIN = 1;  // LED en GPIO 1

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);  // Configurar LED como salida
  
  timer = timerBegin(0, 80, true);  // Inicializar temporizador
  timerAttachInterrupt(timer, &onTimer, true);  // Adjuntar interrupción
  timerAlarmWrite(timer, 1000000, true);  // Configurar alarma cada 1 segundo (1,000,000 µs)
  timerAlarmEnable(timer);  // Habilitar temporizador
}

void loop() {
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    
    totalInterruptCounter++;
    
    // Alternar estado del LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    Serial.print("An interrupt has occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
}


