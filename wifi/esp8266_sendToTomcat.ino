#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "leesuun";       // 여기에 WiFi 네트워크 이름 입력
const char* password = "dl116245";  // 여기에 WiFi 비밀번호 입력
const char* serverIP = "192.168.0.3"; // 톰캣서버 IP주소
const int serverPort = 9001; // 톰캣서버 PORT




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
  // 서버로부터 데이터 수신
  server.handleClient();


  // 서버로 데이터 송신
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    
    // 톰캣 서버로 데이터 전송
    WiFiClient client;
    if (client.connect(serverIP, serverPort)) {
      String url = "/iot/smarthome/tempHumi";
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

void handleData() {
  WiFiClient client;
  String data = server.arg("plain");
  Serial.println(data);
  client.print(data);
  delay(100);
  while (client.available()) {
    String response = client.readStringUntil('\r');
    Serial.print(response);
  }
  client.stop();
  // 여기서 데이터를 처리하는 로직을 추가할 수 있습니다.
  server.send(200, "text/plain", "Data received successfully");
}