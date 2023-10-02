#line 1 "C:\\Users\\sktjs\\OneDrive\\바탕 화면\\iotArdunio\\wifi\\esp8266.c"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "leesuun"; // 여기에 WiFi 네트워크 이름 입력
const char* password = "dl116245"; // 여기에 WiFi 비밀번호 입력


WiFiClient client;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/receive", HTTP_POST, handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleData() {
  String data = server.arg("plain");
  Serial.println(
data);
  client.print(data);
        delay(100);
      while (client.available()) {
        String response = client.readStringUntil('\r');
        Serial.print(response);
      }
  // 여기서 데이터를 처리하는 로직을 추가할 수 있습니다.
  server.send(200, "text/plain", "Data received successfully");
}