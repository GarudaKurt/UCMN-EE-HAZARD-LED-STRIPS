#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3);  // TX = 2, RX = 3

const int btnRight = 5;
const int btnLeft = 6;
const int btnStop = 7;
const int btnHazard = 8;

bool buttonPressedR = false;
bool buttonPressedL = false;
bool buttonPressedS = false;
bool buttonPressedH = false;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnStop, INPUT_PULLUP);
  pinMode(btnHazard, INPUT_PULLUP);
}

void loop() {
  checkButton(btnRight, buttonPressedR, "R");
  checkButton(btnLeft, buttonPressedL, "L");
  checkButton(btnStop, buttonPressedS, "S");
  checkButton(btnHazard, buttonPressedH, "H");

  delay(50);
}

void checkButton(int pin, bool &buttonState, const char *message) {
  if (digitalRead(pin) == LOW && !buttonState) {
    delay(50);
    if (digitalRead(pin) == LOW) { 
      buttonState = true;
      BTSerial.println(message);
      Serial.println("BTN " + String(message));
    }
  } else if (digitalRead(pin) == HIGH) {
    buttonState = false;
  }
  if(Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    BTSerial.println(command);
    Serial.print("Command send: ");
    Serial.println(command);
  }
}
