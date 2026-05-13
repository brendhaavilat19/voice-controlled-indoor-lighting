#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

// =======================
// CONFIGURACIÓN WIFI
// =======================

const char* ssid = "WIFI";
const char* password = "123456789";

// =======================
// SERVIDOR WEB
// =======================

WebServer server(80);

// =======================
// MATRIZ RGB
// =======================

#define PIN 5
#define NUM_LEDS 16

Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// =======================
// FUNCIÓN RGB
// =======================

void setColor(int r, int g, int b) {

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  strip.show();
}

// =======================
// PETICIONES HTTP
// =======================

void handleRoot() {

  Serial.println("Petición recibida");

  if (server.hasArg("r") &&
      server.hasArg("g") &&
      server.hasArg("b")) {

    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();

    Serial.println("RGB:");
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);

    setColor(r, g, b);

    server.send(200, "text/plain", "Color aplicado");
  }

  else {

    server.send(200, "text/plain", "LUMIGARDEN RGB listo");
  }
}

// =======================
// SETUP
// =======================

void setup() {

  Serial.begin(115200);

  // Inicializar matriz
  strip.begin();
  strip.show();

  // WiFi
  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Servidor
  server.on("/", handleRoot);

  server.begin();

  Serial.println("Servidor iniciado");
}

// =======================
// LOOP
// =======================

void loop() {

  server.handleClient();
}
