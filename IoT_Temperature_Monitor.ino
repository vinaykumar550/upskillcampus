#include <ESP8266WiFi.h>
#include <DHT.h>

// Wi-Fi Credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// DHT Sensor Settings
#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  dht.begin();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  WiFiClient client = server.available();

  if (client) {

    while (!client.available()) {
      delay(1);
    }

    client.readStringUntil('\r');

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");

    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><title>IoT Monitoring</title></head>");
    client.println("<body>");
    client.println("<h2>Embedded Systems and IoT Project</h2>");
    client.print("<h3>Temperature: ");
    client.print(temperature);
    client.println(" &deg;C</h3>");

    client.print("<h3>Humidity: ");
    client.print(humidity);
    client.println(" %</h3>");

    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();
  }
}
