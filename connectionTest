#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "leesuun";
const char* password = "dl116245";
const char* serverAddress = "http://192.168.0.3:9001/iot/smarthome";  // 스프링 서버 주소
const char* csrfTokenEndpoint = "http://192.168.0.3:9001/csrf-token";  // CSRF 토큰을 얻을 엔드포인트
const char* cookieEndpoint = "http://192.168.0.3:9001/cookie";  // COOKIE 얻을 엔드포인트

WiFiClient client;
String csrfToken;
String cookie;


String getCookie() {
  HTTPClient http;
  http.begin(client, cookieEndpoint);

   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   http.addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
   http.addHeader("Connection", "keep-alive");
  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {

    String cookieValue = http.getString();
    return cookieValue;
  } else {
    return "";
  }
}


String getCsrfToken() {
  HTTPClient http;
  http.begin(client, csrfTokenEndpoint);
  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    return http.getString();
  } else {
    return "";
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;


    // COOKIE 가져오기
    cookie = getCookie();

    // CSRF 토큰 가져오기
    csrfToken = getCsrfToken();

    
    Serial.print("csrftoken: ");
    Serial.println(csrfToken);

    if (csrfToken.length() > 0) {
      // 전송할 데이터 설정
      String postData = "data=Hello, World!";  // 이 부분에 보낼 데이터를 설정하세요

      // CSRF 토큰을 헤더에 추가
      http.begin(client, serverAddress);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      http.addHeader("X-CSRF-TOKEN", csrfToken);

      http.addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
      http.addHeader("Connection", "keep-alive");
      http.addHeader("Cookie", "JSESSIONID="+cookie);

      

      int httpResponseCode = http.POST(postData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.print("Response data: ");
        Serial.println(response);
      } else {
        Serial.print("HTTP Error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
  }

  delay(5000);  // 5초마다 데이터를 보냄
}
