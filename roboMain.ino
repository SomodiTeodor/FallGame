#include "LedControl.h"
#include "MatrixDisplay.h"
#include "ScrollingText.h"
#include "FallGame.h"


const int joystickSwitchPin = 2;
const int joystickXPin = 0;
const int joystickYPin = 1;

void setup() {
    pinMode(joystickSwitchPin, INPUT_PULLUP);
    randomSeed(analogRead(3));
    attachInterrupt(digitalPinToInterrupt(joystickSwitchPin), interruptFunc, FALLING);
    MatrixDisplay::buttonState = 0;
}


void loop() {
    //Initialization of the game
    FallGame game(joystickXPin, joystickYPin, joystickSwitchPin);
    game.play();
}

//Capture the button press
void interruptFunc(){
    if (millis() - MatrixDisplay::lastSwitch > 500){
        MatrixDisplay::buttonState = !(MatrixDisplay::buttonState);
        MatrixDisplay::lastSwitch = millis();
    }
}

