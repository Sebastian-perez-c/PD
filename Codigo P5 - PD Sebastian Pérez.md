# Practica 5

## Codigo ejercicio 1

```c
#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin();           // Inicializa el bus I2C
  Serial.begin(115200);   // Inicia la comunicación serie
  while (!Serial);        // Espera a que el monitor serie esté listo
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } 
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Scan complete.\n");
  }

  delay(5000);  // Espera 5 segundos antes de volver a escanear
}

```

## Ejercicio 2

```c
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // Dirección I2C de la pantalla

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Inicializar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Error al iniciar SSD1306");
    for (;;);
  }

  display.clearDisplay();   // Limpiar pantalla
  display.setTextSize(2);   // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color blanco

  // Mostrar "Hola" en la primera línea
  display.setCursor(20, 1);
  display.println("Hola");

  // Mostrar "P5" en la segunda línea
  display.setCursor(100, 15);
  display.println("P5");

  display.display(); // Actualizar pantalla
}

void loop() {
  // No hace falta repetir, la pantalla mantiene el texto
}
```

## Ejercicio Frecuencia Cardiaca y Oxígeno

```c
#include <Wire.h>
#include <MAX30105.h>
#include "spo2_algorithm.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;

uint32_t irBuffer[100];
uint32_t redBuffer[100];
int32_t bufferLength;
int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Error inicializando pantalla OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando sensor...");
  display.display();

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 no detectado");
    while (1);
  }

  particleSensor.setup(60, 4, 2, 100, 411, 4096);
  delay(1000);
}

void loop() {
  bufferLength = 100;

  for (byte i = 0; i < bufferLength; i++) {
    while (!particleSensor.available()) particleSensor.check();
    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
  }

  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.print("HR: ");
  display.print(heartRate);
  display.print(" bpm");
  
  display.setCursor(0, 30);
  display.print("SpO2: ");
  display.print(spo2);
  display.println(" %");

  display.display();

  delay(1000); // actualización cada segundo
}
``` 

## Página Web

```c
#include <WiFi.h>
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

WiFiServer server(80);

// Variables globales actualizadas por la medición
volatile int bpm = 0;
volatile int spo2_val = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado. IP:");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        client.readStringUntil('\r');
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();

        client.println("<html><body style='font-family:sans-serif;'>");
        client.println("<h2>Monitor de Salud</h2>");
        client.printf("<p><b>Frecuencia Cardiaca:</b> %d bpm</p>", bpm);
        client.printf("<p><b>Saturacion de Oxigeno:</b> %d %%</p>", spo2_val);
        client.println("</body></html>");

        break;
      }
    }
    client.stop();
  }
}
```