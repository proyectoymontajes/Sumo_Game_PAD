#include <Arduino.h>
#include <BleGamepad.h>  // https://github.com/lemmingDev/ESP32-BLE-Gamepad

BleGamepad bleGamepad;

#define numOfButtons 7
#define enableX true
#define enableY true
#define pinX 15
#define pinY 4



byte previousButtonStates[numOfButtons];
byte currentButtonStates[numOfButtons];
byte buttonPins[numOfButtons] = { 32,  33, 25, 26, 27, 12, 17};
byte physicalButtons[numOfButtons] = {0, 1, 2, 3, 4, 5, 6 };

void setup() {
  pinMode(2, OUTPUT);

  for (byte currentPinIndex = 0; currentPinIndex < numOfButtons; currentPinIndex++) {
    pinMode(buttonPins[currentPinIndex], INPUT_PULLUP);
    previousButtonStates[currentPinIndex] = HIGH;
    currentButtonStates[currentPinIndex] = HIGH;
  }

  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setAutoReport(false);
  bleGamepadConfig.setButtonCount(numOfButtons);
  bleGamepad.begin(&bleGamepadConfig);
  bleGamepadConfig.setAxesMin(0x8001);
  bleGamepadConfig.setAxesMax(0x7FFF);

  // changing bleGamepadConfig after the begin function has no effect, unless you call the begin function again
}

void loop() {
    long anguloX = analogRead(pinX);
    long anguloY = analogRead(pinY);
  if (bleGamepad.isConnected()) {
    digitalWrite(2, HIGH);
    delay(10);


    int ejeX = map(anguloX, 0, 4095, 0, 32767);
    int ejeY = map(anguloY, 0, 4095, 0, 32767);
    
    bleGamepad.setX(ejeX);
    bleGamepad.sendReport();
    bleGamepad.setY(ejeY);
    blesGamepad.sendReport();


    if (ejeX < 18000) {
    bleGamepad.setY(16384);
    bleGamepad.sendReport();
    };
    if (ejeY < 18000) {
    bleGamepad.setX(16384);
    bleGamepad.sendReport();
    };
    
    if (ejeX == 32762) {
    bleGamepad.setY(16384);
    bleGamepad.sendReport();
    };
    if (ejeY == 32762) {
    bleGamepad.setX(16384);
    bleGamepad.sendReport();
    };S
    
    for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++) {
      currentButtonStates[currentIndex] = digitalRead(buttonPins[currentIndex]);

      if (currentButtonStates[currentIndex] != previousButtonStates[currentIndex]) {
        if (currentButtonStates[currentIndex] == LOW) {
          bleGamepad.press(physicalButtons[currentIndex]);
        } else {
          bleGamepad.release(physicalButtons[currentIndex]);
        }
      }
    }

    if (currentButtonStates != previousButtonStates) {
      for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++) {
        previousButtonStates[currentIndex] = currentButtonStates[currentIndex];
      }

      bleGamepad.sendReport();
    }



    delay(20);
  }
}
