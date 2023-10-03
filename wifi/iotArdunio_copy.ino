#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include "./header/LiquidCrystal_I2C.h"
#include "./header/DHT.h"
#include "./header/melody.h"


DHT dht(dhtPin, DHT11);
LiquidCrystal_I2C lcd(lcdAddr, 16, 2);
SoftwareSerial BTSerial(4, 5);
Servo myservo;

unsigned long previousMillis = 0;


boolean blind = OFF;
boolean TV = OFF;

// 현재 LED의 상태를 저장하는 배열. 0일경우 OFF, 1일 경우 ON
byte sensor[] = {0, 0, 0, 0, 0, 0, 0};

float temperature = 0;
float humidity = 0;


void setup() {
  BTSerial.begin(9600);
  dht.begin();
  lcd.init();
  // 모든 LED핀을 출력모드로 설정
  for (int i = 7; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
}


void loop() {
  if (BTSerial.available()) {
    char input = BTSerial.read();

    char receivedChar = Serial.read(); // 시리얼로부터 데이터 읽기
    Serial.print("Received: ");
    Serial.println(receivedChar);

    switch (input) {
      case 'r':                          //RGB LED Red
        setToggle(ledR1, sensor[0]);
        BTSerial.print("8번 핀 상태: ");
        BTSerial.println(digitalRead(ledR1));
        break;

      case 'g':                          //RGB LED Green
        setToggle(ledG1, sensor[1]);
        BTSerial.print("9번 핀 상태: ");
        BTSerial.println(digitalRead(ledG1));
        break;

      case 'b':                          //RGB LED Blue
        setToggle(ledB1, sensor[2]);
        BTSerial.print("10번 핀 상태: ");
        BTSerial.println(digitalRead(ledB1));
        break;

      case 'R':                          //RGB LED1 Red
        setToggle(ledR2, sensor[3]);
        BTSerial.print("11번 핀 상태: ");
        BTSerial.println(digitalRead(ledR2));
        break;

      case 'G':                          //RGB LED1 Green
        setToggle(ledG2, sensor[4]);
        BTSerial.print("12번 핀 상태: ");
        BTSerial.println(digitalRead(ledG2));
        break;

      case 'B':                          //RGB LED1 Blue
        setToggle(ledB2, sensor[5]);
        BTSerial.print("13번 핀 상태: ");
        BTSerial.println(digitalRead(ledB2));
        break;

      case 'aircon':                          //aircon LED
        setToggle(led, sensor[6]);
        break;
    }
  }

  //================2초마다 온습도 측정================//
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {   //interval(2s)마다 온습도 측정
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
