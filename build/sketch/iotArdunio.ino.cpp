#include <Arduino.h>
#line 1 "C:\\Users\\sktjs\\OneDrive\\바탕 화면\\iotArdunio\\iotArdunio.ino"
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "DHT.h"
#include "melody.h"


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


#line 26 "C:\\Users\\sktjs\\OneDrive\\바탕 화면\\iotArdunio\\iotArdunio.ino"
void setup();
#line 37 "C:\\Users\\sktjs\\OneDrive\\바탕 화면\\iotArdunio\\iotArdunio.ino"
void loop();
#line 138 "C:\\Users\\sktjs\\OneDrive\\바탕 화면\\iotArdunio\\iotArdunio.ino"
void setToggle(int PIN, byte& stat);
#line 150 "C:\\Users\\sktjs\\OneDrive\\바탕 화면\\iotArdunio\\iotArdunio.ino"
void moveBlind(int angle);
#line 26 "C:\\Users\\sktjs\\OneDrive\\바탕 화면\\iotArdunio\\iotArdunio.ino"
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

    switch (input) {
      case 'a':                          //RGB LED Red
        setToggle(ledR1, sensor[0]);
        break;

      case 'b':                          //RGB LED Green
        setToggle(ledG1, sensor[1]);
        break;

      case 'c':                          //RGB LED Blue
        setToggle(ledB1, sensor[2]);
        break;

      case 'd':                          //RGB LED1 Red
        setToggle(ledR2, sensor[3]);
        break;

      case 'e':                          //RGB LED1 Green
        setToggle(ledG2, sensor[4]);
        break;

      case 'f':                          //RGB LED1 Blue
        setToggle(ledB2, sensor[5]);
        break;

      case 'g':                          //aircon LED
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

    if (BTSerial.available()) {
      BTSerial.print(temperature);
      BTSerial.print(humidity);

    }

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

