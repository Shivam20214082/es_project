#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "Tp";  // Your WiFi SSID
const char *serverURL = "http://192.168.43.184/es_project/test_data.php";

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  connectWiFi();  // Call the connectWiFi function in setup
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (Serial.available() > 0) {
      String arduinoData = Serial.readStringUntil('\n');
      Serial.println("Data available from Arduino: " + arduinoData);
      sendToServer(arduinoData);  // Call the sendToServer function
    }
  } else {
    Serial.println("Not connected to WiFi");
    connectWiFi();  // Reconnect if not connected
  }
}

void connectWiFi() {
  WiFi.begin(ssid);
  Serial.println("Connecting to WiFi...");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void sendToServer(String data) {
  Serial.println("Sending data to server...");
  WiFiClient client;  // Use WiFiClient instead of HTTPClient

  HTTPClient http;
  http.begin(client, serverURL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST("data=" + data);  

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Server response: " + payload);
  } else {
    Serial.println("HTTP error code: " + String(httpCode));
  }

  http.end();
}
