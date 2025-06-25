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

  Wire.begin(4, 17);  // SDA = GPIO 8, SCL = GPIO 9

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
