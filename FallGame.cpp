#include "Arduino.h"
#include "FallGame.h"
#include "MatrixDisplay.h"
#include "ScrollingText.h"
#include <string.h>

FallGame::FallGame(int joyXPin, int joyYPin, int joySwitchPin) {
    joystickXPin = joyXPin;
    joystickYPin = joyYPin;
    joystickSwitchPin = joySwitchPin;

    reset();
}

void FallGame::reset() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            frame[i][j] = 0;

    playerPosX = 3;
    playerPosY = 1;
    difficulty = 0;
    score = 0;
    deltaX = 3;
    gameState = 0;
    currentTime = 0;
    oldTime = 0;
    lastBlink = 0;
    lastPositionCheck = 0;
    toAppend = B11000011;
}

//Read input from the joystick
void FallGame::getPlayerPosition() {
    currentTime = millis();
    if (currentTime - lastPositionCheck >= moveDelayTime) {
        int joystickSwitchValue = !(digitalRead(joystickSwitchPin));
        int joystickXValue = analogRead(joystickYPin);
        int joystickYValue = analogRead(joystickXPin);
        //joystickXValue = map(joystickXValue, 0, 1023, 1023, 0);

        //Turn off player led, move, turn on player led
        int playerValue = frame[playerPosY][playerPosX];
        frame[playerPosY][playerPosX] = 0;
        if (joystickXValue < 300) {
            playerPosX --;
        }
        else if (joystickXValue > 700){
            playerPosX ++;
        }
        if (playerPosX < 0) playerPosX = 0;
        else if (playerPosX > 7) playerPosX = 7;
        
        //Check for collision
        if (frame[playerPosY][playerPosX] != 0) {
            gameState = 2;
        }
        frame[playerPosY][playerPosX] = playerValue;
        lastPositionCheck = currentTime;
    }
}

void FallGame::shiftMatrixUp(int matrix[8][8]) {
    int i, j;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 8; j++) {
            matrix[i][j] = matrix [i + 1][j];
        }
    }
    for (j = 0; j < 8; j++) {
        matrix[7][j] = 0;
    }
}

void FallGame::display() {
    MatrixDisplay::getInstance()->display(frame);
}

//Begin the game
void FallGame::play() {
    reset();
    display();

    //Wait for player input to begin the game
    while (gameState == 0) {
        ScrollingText scroller;
        int aux = scroller.scrollText("PRESS TO START   ");
        if(aux == 1) gameState = 1;
    }

    //Actually play the game
    while (gameState == 1) {
        getPlayerPosition();
        update();
        display();
    }

    //Show GAME OVER once, can be skipped by player input
    if (gameState == 2){
        ScrollingText scroller;
        scroller.scrollText(" ");
        scroller.scrollText("GAME OVER! ");
    }

    //Loop the score until player input
    while (gameState == 2){
        String scoreString = String(score, DEC);
        char scoreChar[scoreString.length()+1];
        scoreString.toCharArray(scoreChar, scoreString.length()+1);
        ScrollingText scroller;
        char buffer[100] = {0};
        strcat(buffer,"Scor: ");
        strcat(buffer,scoreChar);
        strcat(buffer,"  \0");
        int aux = scroller.scrollText(buffer);
        if(aux == 1) gameState = 3;
    }
}

void FallGame::update() {
    currentTime = millis();

    //Move the level up, adding a new row and increasing the score
    if (currentTime - oldTime > 300 - difficulty) {
        int playerValue = frame[playerPosY][playerPosX];
        frame[playerPosY][playerPosX] = 0;
        shiftMatrixUp(frame);
        appendRow();
        if (frame[playerPosY][playerPosX] != 0){
            gameState = 2;
            return;
        }
        frame[playerPosY][playerPosX] = playerValue;
        oldTime = currentTime;
        score ++;
    }

    //Blink the player every 60 milliseconds
    if (currentTime - lastBlink > 60) {
        frame[playerPosY][playerPosX] = 1 - frame[playerPosY][playerPosX];
        lastBlink = currentTime;
    }

    //Increase difficulty every second
    if (currentTime - lastDifficultyIncrease >= 1000){
        difficulty += 5;

        //Limit the difficulty
        if (difficulty > 200) difficulty = 200;
        lastDifficultyIncrease = currentTime;
    }
}

/*
 * Function "appendRow()" will generate a new row
 * to be added at the bottom of the frame. The added
 * row will contain the gap with 1 pixel to the left
 * or to the right, or will stay in place, with respect
 * to the previous rows.
 */
void FallGame::appendRow() {
    long randNum = random(3);
    byte andByte;
    int lengthX = 4;

    switch (randNum) {
        case 0:
            //Move gap to the left
            deltaX = deltaX - 1;
            toAppend = toAppend << 1;
            toAppend = toAppend | B00000001;
            if (deltaX <= 1) {
                toAppend = B11111111 >> lengthX;
                deltaX = 1;
            }
            break;
        case 1:
            //Do nothing
            break;
        case 2:
            //Move gap to the right.
            deltaX = deltaX + 1;
            toAppend = toAppend >> 1;
            toAppend = toAppend | B10000000;
            if (deltaX > 8 - lengthX + 1) {
                toAppend = B11111111 << lengthX;
                deltaX = 8 - lengthX + 1;
            }
            break;
    }

    //Add the row
    andByte = B10000000;
    for (int i = 0; i < 8; i++) {
        frame[7][i] = toAppend & andByte;
        andByte = andByte >> 1;
    }
}

