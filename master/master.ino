#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);  // TX = 10, RX = 11

const int btnRight = 5;
const int btnLeft = 6;
const int btnStop = 7;
const int trig = 9;
const int ech = 12; 
const int buzzer = 4;

bool buttonPressedR = false;
bool buttonPressedL = false;
bool buttonPressedS = false;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnStop, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(ech, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void loop() {

  long sensor = Distance();
  if(sensor < 50) {
    Serial.println("Hazard Activate");
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
  }
  checkButton(btnRight, buttonPressedR, "R");
  checkButton(btnLeft, buttonPressedL, "L");
  checkButton(btnStop, buttonPressedS, "S");

  delay(50);
}

long Distance() {

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(ech, HIGH, 20000);
  return duration / 58.2;

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
