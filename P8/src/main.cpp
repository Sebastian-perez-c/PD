#include <Arduino.h>

#define BUF_SIZE 1024
uint8_t data_rx0[BUF_SIZE];
uint8_t data_rx2[BUF_SIZE];

void setup() {
  // Inicializar el monitor serial (UART0)
  Serial.begin(115200);  // UART0 para comunicación con el PC
  
  // Asegúrate de que se está configurando correctamente
  Serial.println("Iniciando la comunicación...");
  
  // Configuración de UART2 usando pines GPIO16 (RXD2) y GPIO17 (TXD2)
  const int RXD2_PIN = 16;
  const int TXD2_PIN = 17;
  Serial2.begin(115200, SERIAL_8N1, RXD2_PIN, TXD2_PIN);  // UART2 con RXD2_PIN y TXD2_PIN
  
  // Confirmación de que UART2 se ha iniciado correctamente
  Serial.println("UART2 configurado y listo para comunicarse.");
}

void loop() {
  // Leer datos de UART0 (RXD0) y reenviar a UART2 (TXD2)
  if (Serial.available()) {
    int len = Serial.readBytes(data_rx0, BUF_SIZE);
    if (len > 0) {
      Serial.println("Datos leídos desde UART0, reenviando a UART2...");
      Serial2.write(data_rx0, len);  // Redirigir datos a UART2
    }
  }

  // Leer datos de UART2 (RXD2) y reenviar a UART0 (TXD0)
  if (Serial2.available()) {
    int len = Serial2.readBytes(data_rx2, BUF_SIZE);
    if (len > 0) {
      Serial.println("Datos leídos desde UART2, reenviando a UART0...");
      Serial.write(data_rx2, len);  // Redirigir datos a UART0
    }
  }
}
