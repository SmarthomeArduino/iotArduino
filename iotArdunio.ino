#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include "./header/LiquidCrystal_I2C.h"
#include "./header/DHT.h"
#include "./header/melody.h"


SoftwareSerial ESP_wifi(2, 3);  // ESP8266 RX, TX
DHT dht(dhtPin, DHT11);
LiquidCrystal_I2C lcd(lcdAddr, 16, 2);
SoftwareSerial BTSerial(4, 5);
Servo myservo;

unsigned long previousMillis = 0;


boolean blind = OFF;
boolean TV = OFF;

// 현재 LED의 상태를 저장하는 배열. 0일경우 OFF, 1일 경우 ON
byte sensor[] = {0, 0, 0, 0, 0, 0, 0};
char buffer[100];

float temperature = 0;
float humidity = 0;


void setup() {
  // BTSerial.begin(9600);
  Serial.begin(115200);
  ESP_wifi.begin(115200);
  delay(1000);
  dht.begin();
  lcd.init();
  // 모든 LED핀을 출력모드로 설정
  for (int i = 7; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
}


void loop() {
  int index = 0;
  while (ESP_wifi.available()) {
    char c = ESP_wifi.read();
    buffer[index] = c;
    index++;
  }
  buffer[index] = '\0';

  
  if (strstr(buffer, "toggleLED") != NULL) {
    char originalString[100];
    strncpy(originalString, buffer, sizeof(buffer));
    char extractedString[3];
    char* delimiter = strchr(originalString, ':');
    if (delimiter != NULL) {
      strncpy(extractedString, delimiter + 1, sizeof(extractedString) - 1);
      extractedString[sizeof(extractedString) - 1] = '\0';
      Serial.print("Extracted String: ");
      Serial.println(extractedString);
    } else {
      Serial.println("Delimiter not found in the original string.");
    }

    if (strcmp(extractedString, "R1") == 0) {
      setToggle(ledR1, sensor[0]);
    } else if (strcmp(extractedString, "G1") == 0) {
      setToggle(ledG1, sensor[1]);
    } else if (strcmp(extractedString, "B1") == 0) {
      setToggle(ledB1, sensor[2]);
    } else if (strcmp(extractedString, "R2") == 0) {
      setToggle(ledR2, sensor[3]);
    } else if (strcmp(extractedString, "G2") == 0) {
      setToggle(ledG2, sensor[4]);
    } else if (strcmp(extractedString, "B2") == 0) {
      setToggle(ledB2, sensor[5]);
    }

    Serial.println("Buffer contains 'toggleLED' string!");
  }

  //================10초마다 온습도 측정================//
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {   //interval(10s)마다 온습도 측정
    previousMillis = currentMillis;
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();


/*
    // 온습도 테스트
    BTSerial.print("temp: ");
    BTSerial.print(temperature,1);
    BTSerial.print(",humi: ");
    BTSerial.print(humidity,0);
    BTSerial.println();  // 개행 문자로 데이터를 종료
    BTSerial.print("cds: ");
    BTSerial.print(analogRead(cdsPin));
    BTSerial.print(", IR: ");
    BTSerial.print(analogRead(IRPin));
    BTSerial.println();  // 개행 문자로 데이터를 종료
*/

    if (TV == ON) {                                  //TV가 ON상태이면 온습도 값 갱신
      lcd.setCursor(0, 0);
      lcd.print("Temp : ");
      lcd.print(temperature);
      lcd.setCursor(0, 1);
      lcd.print("humi : ");
      lcd.print(humidity);
    }
  }
  //===================================================//


  //================블라인드 제어================//
  int cdsVal = analogRead(cdsPin);
  if (cdsVal > refVal && blind == OFF) {      //현재 어둡고, 블라인드가 닫혀있다면 블라인드 Open
    ascMelody();
    moveBlind(blindOpen);
    blind = ON;
  }
  else if (cdsVal < refVal && blind == ON) {   //현재 밝고, 블라인드가 열려있다면 블라인드 Close
    dscMelody();
    moveBlind(blindClose);
    blind = OFF;
  }
  //================블라인드 제어 끝================//





  //================TV 제어================//
  int IRval = analogRead(IRPin);
  if (IRval < 100 && TV == ON) {          //TV가 켜져있고, 거리가 가깝다면 TV OFF
    TV = OFF;
    lcd.clear();
    lcd.noBacklight();
  }

  else if (IRval > 900 && TV == OFF) {    //TV가 꺼져있고, 거리가 멀다면 TV ON
    TV = ON;
    lcd.backlight();
  }
  //================TV 제어 끝================//
}




//LED 토글함수
void setToggle(int PIN, byte& stat) {
  if (stat == OFF) {
    digitalWrite(PIN, HIGH);
    stat = ON;
  }
  else {
    digitalWrite(PIN, LOW);
    stat = OFF;
  }
}

//블라인드 제어
void moveBlind(int angle) {
  myservo.attach(servoPin);
  myservo.write(angle);
  delay(300);
  myservo.detach();
}
