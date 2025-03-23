# Sebastian Pérez

# Códigos P2 - PD

## 1er Código

```cpp
#include <Arduino.h>
struct Button {
 const uint8_t PIN;
 uint32_t numberKeyPresses;
 bool pressed;
};
Button button1 = {18, 0, false};
void IRAM_ATTR isr() {
 button1.numberKeyPresses += 1;
 button1.pressed = true;
}
void setup() {
 Serial.begin(115200);
 pinMode(button1.PIN, INPUT_PULLUP);
 attachInterrupt(button1.PIN, isr, FALLING);
}
void loop() {
 if (button1.pressed) {
 Serial.printf("Button 1 has been pressed %u times\n",
 button1.numberKeyPresses);
 button1.pressed = false;
}
//Detach Interrupt after 1 Minute
static uint32_t lastMillis = 0;
 if (millis() - lastMillis > 60000) {
 lastMillis = millis();
 detachInterrupt(button1.PIN);
 Serial.println("Interrupt Detached!");
}
}
``` 

## 2do Código

```cpp
#include <Arduino.h>

volatile int interruptCounter;
int totalInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
    Serial.begin(115200);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 4000000, true);
    timerAlarmEnable(timer);
}
void loop() {
    if (interruptCounter > 0) {
        portENTER_CRITICAL(&timerMux);
        interruptCounter--;
        portEXIT_CRITICAL(&timerMux);
    totalInterruptCounter++;

    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
        }
}

```

## Código ejercicio subida de nota.

```cpp
#include <Arduino.h>

#define LED_PIN 2
#define BUTTON_UP 4
#define BUTTON_DOWN 5

hw_timer_t *timer = NULL;
volatile int blinkInterval = 500;  // Tiempo inicial en ms
volatile bool ledState = false;
volatile unsigned long lastPressUp = 0;
volatile unsigned long lastPressDown = 0;
const int debounceTime = 50;

void IRAM_ATTR onTimer() {
    static unsigned long lastToggle = 0;
    unsigned long now = millis();
    if (now - lastToggle >= blinkInterval) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        lastToggle = now;
    }

    // Lectura y filtrado de rebotes de los botones
    if (digitalRead(BUTTON_UP) == LOW && (now - lastPressUp > debounceTime)) {
        if (blinkInterval > 100) blinkInterval -= 50;  // Aumenta la frecuencia
        lastPressUp = now;
    }
    if (digitalRead(BUTTON_DOWN) == LOW && (now - lastPressDown > debounceTime)) {
        if (blinkInterval < 2000) blinkInterval += 50;  // Disminuye la frecuencia
        lastPressDown = now;
    }
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);

    timer = timerBegin(0, 80, true);  // Timer 0, divisor de 80 -> 1 tick = 1µs
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10000, true);  // Interrumpe cada 10 ms
    timerAlarmEnable(timer);
}

void loop() {
    // El control lo gestiona la interrupción, el loop puede quedar vacío
}

```