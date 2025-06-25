#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>

// Credenciales de la red WiFi
const char* ssid = "Nautilus"; 
const char* password = "20000Leguas"; 

// Creación del servidor en el puerto 80
WebServer server(80);

// Prototipo de la función handle_root
void handle_root() {
  String HTML = "<!DOCTYPE html>\
  <html>\
  <head>\
      <meta charset='UTF-8'>\
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
      <title>ESP32 WebServer</title>\
      <style>\
          body {\
              font-family: Arial, sans-serif;\
              text-align: center;\
              margin: 50px;\
              animation: changeBackground 10s infinite;\
              position: relative;\
          }\
          h1 {\
              color: #4CAF50;\
              animation: fadeIn 3s ease-in-out;\
          }\
          .dinosaurio {\
              position: relative;\
              width: 150px;\
              height: 100px;\
              background-color: #4CAF50;\
              border-radius: 10px;\
              margin: 20px auto;\
          }\
          .dinosaurio .cabeza {\
              position: absolute;\
              top: -10px;\
              left: 20px;\
              width: 50px;\
              height: 50px;\
              background-color: #2c6b31;\
              border-radius: 50%;\
          }\
          .dinosaurio .piernas {\
              position: absolute;\
              bottom: -10px;\
              left: 25px;\
              width: 100px;\
              height: 20px;\
              background-color: #2c6b31;\
              border-radius: 5px;\
          }\
          .dinosaurio .cola {\
              position: absolute;\
              bottom: 10px;\
              right: -40px;\
              width: 40px;\
              height: 15px;\
              background-color: #2c6b31;\
              border-radius: 15px;\
              transform: rotate(45deg);\
          }\
          .sun {\
              position: absolute;\
              bottom: 10px;\
              left: 50%;\
              transform: translateX(-50%);\
              width: 80px;\
              height: 80px;\
              background-color: #FFD700;\
              border-radius: 50%;\
              animation: sunRiseAndSet 10s infinite;\
          }\
          @keyframes fadeIn {\
              0% { opacity: 0; }\
              100% { opacity: 1; }\
          }\
          @keyframes changeBackground {\
              0% { background-color: #ffb3b3; }\
              25% { background-color: #ffcc99; }\
              50% { background-color: #99ccff; }\
              65% { background-color:rgb(0, 0, 0); }\
              75% { background-color: #c2ff99; }\
              85% { background-color:rgb(254, 0, 0); }\
              100% { background-color: #ffb3b3; }\
          }\
          @keyframes sunRiseAndSet {\
              0% { bottom: -1000px; }\
              25% { bottom: 1000px; }\
              50% { bottom: 1000px; }\
              65% { bottom: -1000px; }\
              75% { bottom: -1000px; }\
              85% { bottom: 1000px; }\
              100% { bottom: -1000px; }\
          }\
      </style>\
  </head>\
  <body>\
      <h1>¡Un Dinosaurio en la ONU! 🦖🌍</h1>\
      <div class='dinosaurio'>\
          <div class='cabeza'></div>\
          <div class='piernas'></div>\
          <div class='cola'></div>\
      </div>\
      <div class='sun'></div>\
      <p>¡Bienvenido a mi página web con ESP32! 🌐</p>\
  </body>\
  </html>";
  
  server.send(200, "text/html", HTML);  // Enviar la respuesta con el HTML y estilo
}
// Configuración inicial del ESP32
void setup() {
    Serial.begin(115200);
    Serial.println("Conectando a WiFi...");

    WiFi.begin(ssid, password);
    
    // Esperar conexión WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado con éxito.");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());  // Muestra la IP asignada

    // Configuración del servidor
    server.on("/", handle_root);
    server.begin();
    Serial.println("Servidor HTTP iniciado.");
}

// Bucle principal del ESP32
void loop() {
    server.handleClient();
}

