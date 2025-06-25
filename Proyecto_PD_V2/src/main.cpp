#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_VL53L0X.h>
#include <Audio.h>               // ESP32-audioI2S
#include <SD.h>
#include <FS.h>
#include <SPI.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_SDA 4
#define OLED_SCL 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- VL53L0X ----------
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// ---------- SD ----------
#define SD_CS 15
#define SD_MOSI 18
#define SD_MISO 17
#define SD_SCK  16

// ---------- BOTONES ----------
#define BTN_VOL_UP   12
#define BTN_VOL_DOWN 14

// ---------- AUDIO ----------
Audio audio;

// ---------- VARIABLES ----------
#define MAX_CANCIONES 20
String canciones[MAX_CANCIONES];
int totalCanciones = 0;
int currentSong = 0;
float volume = 0.5; // 0.0 a 1.0

// ---------- FUNCIONES ----------
void mostrarInfo(String titulo, String artista) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Reproductor MP3");
  display.println("Cancion:");
  display.println(titulo);
  display.println("Artista:");
  display.println(artista);
  display.display();
}

void reproducirCancion(String path) {
  audio.stopSong();
  audio.connecttoFS(SD, path.c_str());
  audio.setVolume(volume * 21);
  mostrarInfo(path, "Artista X");
}

void escanearCanciones(fs::FS &fs, const char *dirname) {
  File root = fs.open(dirname);
  if (!root || !root.isDirectory()) {
    Serial.println("Directorio inválido");
    return;
  }

  while (true) {
    File file = root.openNextFile();
    if (!file) break;

    String nombre = file.name();
    if (!file.isDirectory() && nombre.endsWith(".wav")) {
      if (totalCanciones < MAX_CANCIONES) {
        canciones[totalCanciones++] = "/" + nombre;
        Serial.println("Encontrado: " + canciones[totalCanciones - 1]);
      }
    }
    file.close();
  }
}

// ---------- SETUP ----------
void setup() {
  Wire.begin(OLED_SDA, OLED_SCL);
  Serial.begin(115200);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Fallo al iniciar el OLED");
    while (1);
  }

  // VL53L0X
  if (!lox.begin()) {
    Serial.println("Fallo al iniciar el VL53L0X");
    while (1);
  }

  // BOTONES
  pinMode(BTN_VOL_UP, INPUT_PULLUP);
  pinMode(BTN_VOL_DOWN, INPUT_PULLUP);

  // SD
  if (!SD.begin(SD_CS)) {
    Serial.println("No se pudo montar la tarjeta SD");
    while (1);
  }

  escanearCanciones(SD, "/");
  if (totalCanciones == 0) {
    Serial.println("No hay canciones .wav en la SD");
    while (1);
  }

  // AUDIO
  audio.setPinout(46, 3, 29); // Ajusta a tu conexión I2S
  audio.setVolume(volume * 21);
  reproducirCancion(canciones[currentSong]);
}

// ---------- LOOP ----------
void loop() {
  audio.loop();

  // Volumen
  if (digitalRead(BTN_VOL_UP) == LOW) {
    volume = min(1.0, volume + 0.1);
    audio.setVolume(volume * 21);
    Serial.println("Volumen: " + String(volume, 1));
    delay(200);
  }
  if (digitalRead(BTN_VOL_DOWN) == LOW) {
    volume = max(0.0, volume - 0.1);
    audio.setVolume(volume * 21);
    Serial.println("Volumen: " + String(volume, 1));
    delay(200);
  }

  // Sensor de proximidad: cambio de canción
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus == 0 && measure.RangeMilliMeter < 100) {
    currentSong = (currentSong + 1) % totalCanciones;
    reproducirCancion(canciones[currentSong]);
    delay(1000); // evitar doble lectura por un solo gesto
  }
}
