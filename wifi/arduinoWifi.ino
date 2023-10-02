#include <SoftwareSerial.h>

SoftwareSerial ESP_wifi(2, 3);  // ESP8266 RX, TX

int ON = 1;
int OFF = 0;
int airCon = 7;
int ledR1 = 8;
int ledG1 = 9;
int ledB1 = 10;
int ledR2 = 11;
int ledG2 = 12;
int ledB2 = 13;
byte sensor[] = { 0, 0, 0, 0, 0, 0, 0 };

char buffer[100];

void setup() {
  Serial.begin(115200);
  ESP_wifi.begin(115200);
  delay(1000);
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

  delay(5000);
}

void setToggle(int PIN, byte& stat) {
  if (stat == OFF) {
    digitalWrite(PIN, HIGH);
    stat = ON;
  } else {
    digitalWrite(PIN, LOW);
    stat = OFF;
  }
}
