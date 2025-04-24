## Sebastian Pérez

# Código Práctica 6.

## Ejercio 1.

```c

#include <SPI.h>
#include <SD.h>

// Prueba con estos pines alternativos:
#define CS_PIN    5   // Opciones comunes: 5, 13, 21
#define SCK_PIN   12
#define MISO_PIN  13
#define MOSI_PIN  11

SPIClass spi = SPIClass(FSPI); // Usa SPI1 (FSPI)
File myFile;

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Configura SPI con los pines
  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  Serial.print("Iniciando SD... ");
  
  // Intenta inicializar con velocidad reducida (1 MHz)
  if (!SD.begin(CS_PIN, spi, 1000000)) { // <- ¡Velocidad reducida!
    Serial.println("Fallo. Verifica:");
    Serial.println("- Pines y conexiones");
    Serial.println("- Formato FAT32");
    Serial.println("- Resistencia pull-up");
    while (1);
  }
  Serial.println("Correcto");

  // Leer archivo...
  myFile = SD.open("/myFile.txt");
  if (myFile) {
    Serial.println("Contenido:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo");
  }
}

void loop() {}

```

## Ejercicio 2.

```c

#include <SPI.h>
#include <MFRC522.h>

// Pines SPI (ESP32-S3)
#define RFID_CS_PIN   5   // GPIO5
#define RST_PIN       4   // GPIO4

MFRC522 rfid(RFID_CS_PIN, RST_PIN); // Constructor

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Inicializa SPI con pines personalizados para ESP32-S3:
  SPI.begin(12, 13, 11, RFID_CS_PIN); // SCK=12, MISO=13, MOSI=11, SS=5

  rfid.PCD_Init(); 
  Serial.println("Acerca una tarjeta RFID...");

}

void loop() {
  // Detección mejorada
  if (!rfid.PICC_IsNewCardPresent()) {
    return; 
  }
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Mostrar UID
  Serial.print("UID:");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

```

## Ejercicios de subida de nota.

### Parte 1.

```c

#include <SPI.h>
#include <MFRC522.h>
#include <SD.h>
#include <RTClib.h> // Librería para RTC (opcional si se usa un módulo de reloj como DS3231)

// Pines SPI compartidos
#define SCK_PIN    12
#define MISO_PIN   13
#define MOSI_PIN   11
#define SD_CS_PIN   5
#define RFID_CS_PIN 21
#define RST_PIN     4

SPIClass spi = SPIClass(FSPI); // Bus SPI1
MFRC522 rfid(RFID_CS_PIN, RST_PIN);
RTC_DS3231 rtc;
File logFile;

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Iniciar SPI
  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, -1); // CS se controla manualmente

  // Inicializar SD
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH); // Desactivar SD
  if (!SD.begin(SD_CS_PIN, spi)) {
    Serial.println("Error al iniciar tarjeta SD");
    while (true);
  }

  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println("Error RTC");
    while (true);
  }

  // Inicializar RFID
  pinMode(RFID_CS_PIN, OUTPUT);
  digitalWrite(RFID_CS_PIN, HIGH); // Desactivar RFID
  rfid.PCD_Init();

  Serial.println("Sistema listo. Acerca una tarjeta RFID.");
}

void loop() {
  digitalWrite(RFID_CS_PIN, LOW);   // Activar RFID
  digitalWrite(SD_CS_PIN, HIGH);    // Desactivar SD

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    digitalWrite(RFID_CS_PIN, HIGH); // Desactivar RFID
    return;
  }

  // Obtener UID
  String uidStr = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidStr += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  digitalWrite(RFID_CS_PIN, HIGH); // Desactivar RFID

  // Obtener fecha/hora
  DateTime now = rtc.now();
  String timestamp = String(now.timestamp());

  // Escribir en archivo log
  digitalWrite(SD_CS_PIN, LOW);   // Activar SD
  logFile = SD.open("/rfid_log.txt", FILE_APPEND);
  if (logFile) {
    logFile.print(timestamp);
    logFile.print(" - UID: ");
    logFile.println(uidStr);
    logFile.close();
    Serial.println("Log guardado: " + timestamp + " UID: " + uidStr);
  } else {
    Serial.println("No se pudo escribir en el archivo");
  }
  digitalWrite(SD_CS_PIN, HIGH);  // Desactivar SD

  delay(1000);
}

```

### Parte 2. (Página web)

```c

#include <WiFi.h>
#include <WebServer.h>
#include <SD.h>
#include <SPI.h>

#define SD_CS_PIN 5
#define SCK_PIN   12
#define MISO_PIN  13
#define MOSI_PIN  11

const char* ssid = "TU_SSID";
const char* password = "TU_CONTRASEÑA";

WebServer server(80);
SPIClass spi = SPIClass(FSPI);

void handleRoot() {
  String page = "<html><head><title>Lecturas RFID</title></head><body><h1>Registro RFID</h1><pre>";
  File file = SD.open("/rfid_log.txt");
  if (file) {
    while (file.available()) {
      page += (char)file.read();
    }
    file.close();
  } else {
    page += "No se pudo abrir el archivo.";
  }
  page += "</pre></body></html>";
  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi");
  Serial.println(WiFi.localIP());

  // Inicializar SD
  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SD_CS_PIN);
  if (!SD.begin(SD_CS_PIN, spi)) {
    Serial.println("Fallo al iniciar SD");
    return;
  }

  // Servidor web
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}

```
