//ESP8266 WIFI

#include <ESP8266WiFi.h>

const char* ssid = "leesuun";
const char* password = "dl116245";
const char* serverIP = "192.168.0.3";
const int serverPort = 9001;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // 아두이노로부터 데이터를 받아옴
  // 예제에서는 LED 제어 데이터를 받아온다고 가정
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    
    // 톰캣 서버로 데이터 전송
    WiFiClient client;
    if (client.connect(serverIP, serverPort)) {
      String url = "/iot/controlLED";
      String request = "POST " + url + " HTTP/1.1\r\n" +
                       "Host: " + serverIP + "\r\n" +
                       "Content-Type: application/x-www-form-urlencoded\r\n" +
                       "Content-Length: " + data.length() + "\r\n\r\n" +
                       data;
      // 톰캣 서버로 전송하는 코드	
      client.print(request);
      delay(100);
      while (client.available()) {
        String response = client.readStringUntil('\r');
        Serial.print(response);
      }
      client.stop();
    }else{
      Serial.println("failed connect..");
    }
  }
}
