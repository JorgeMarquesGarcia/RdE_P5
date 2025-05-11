#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "XXXXXXXXXXXXXX";
const char* password = "XXXXXXXXXXXXX";

const char* mqtt_server = "io.adafruit.com";
const int mqtt_port = 1883;
const char* mqtt_user = "JmarquesGarcia";
const char* mqtt_pass = "XXXXXXXXXXXXX";
const char* mqtt_client_id = "ArduinoClient";

const char* mqtt_topic = "JmarquesGarcia/feeds/p5-rde";

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_pass)) {
      Serial.println("Conectado");
      client.subscribe(mqtt_topic);  // Suscribirse al feed
      client.publish(mqtt_topic, "ESP32 conectado con PubSubClient");
      delay(5000);
      client.publish(mqtt_topic, "ESP32 aún sigue conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5s");
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
