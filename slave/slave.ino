//Initializedlibrary
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
//Inialize bluetooth commnunication
SoftwareSerial BTSerial(2, 3);  // TX = 2, RX = 3
//Pin initialized
#define LED_PIN_LEFT    4
#define LED_COUNT_LEFT  14
#define LED_PIN_RIGHT   5
#define LED_COUNT_RIGHT 14
//Interval
#define BLINK_DURATION 10000 
#define BLINK_INTERVAL 500
//Global variable initialized
char activeCommand = ' ';
unsigned long blinkStartTime = 0;
bool isBlinking = false;
//Global function for LED Strip
Adafruit_NeoPixel stripLeft(LED_COUNT_LEFT, LED_PIN_LEFT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripRight(LED_COUNT_RIGHT, LED_PIN_RIGHT, NEO_GRB + NEO_KHZ800);
//Activate left
void setColor_Left(int r, int g, int b) {
  for (int i = 0; i < stripLeft.numPixels(); i++) {
    stripLeft.setPixelColor(i, stripLeft.Color(r, g, b));
  }
  stripLeft.show();
}
//Activate right
void setColor_Right(int r, int g, int b) {
  for (int i = 0; i < stripRight.numPixels(); i++) {
    stripRight.setPixelColor(i, stripRight.Color(r, g, b));
  }
  stripRight.show();
}
//Initialize 
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  stripLeft.begin();
  stripRight.begin();
  stripLeft.show();
  stripRight.show();
  Serial.println("Slave is ready!");
}
//Iterate the code
void loop() {
  //check if the buttons events activate
  if (BTSerial.available()) {
    char receivedChar = BTSerial.read();

    if (receivedChar == '\r' || receivedChar == '\n.')
      return;

    Serial.print("Received: ");
    Serial.println(receivedChar);

    activeCommand = receivedChar;
    blinkStartTime = millis();
    isBlinking = true;
  }
  //Activate state for LED
  if (isBlinking) {
    //blinking for 10 seconds, or overwrite the current state
    unsigned long elapsedTime = millis() - blinkStartTime;
    if (elapsedTime < BLINK_DURATION) {
      if ((elapsedTime / BLINK_INTERVAL) % 2 == 0) {
        if (activeCommand == 'R') {
          setColor_Right(0, 255, 0); // Right strip Green
          setColor_Left(0, 0, 0);    // Left strip Off
        } else if (activeCommand == 'L') {
          setColor_Left(0, 255, 0);  // Left strip Green
          setColor_Right(0, 0, 0);   // Right strip Off
        } else if (activeCommand == 'S') {
          setColor_Left(255, 0, 0);  // Both strips Red
          setColor_Right(255, 0, 0);
        }
      } else {
        setColor_Left(0, 0, 0);
        setColor_Right(0, 0, 0);
      }
    } else {
      setColor_Left(0, 0, 0); // turn off after 10 seconds
      setColor_Right(0, 0, 0);
      isBlinking = false;
    }
  }

  delay(100);
}
