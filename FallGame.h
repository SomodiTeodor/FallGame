#ifndef FALL_GAME_H
#define FALL_GAME_H

#include "Arduino.h"
#include "MatrixDisplay.h"
#include "ScrollingText.h"
#include <string.h>

class FallGame {
    private:
        int joystickXPin;
        int joystickYPin;
        int joystickSwitchPin;

        int deltaX;

        byte toAppend;
        int frame[8][8];
        int playerPosX;
        int playerPosY;
        int difficulty;
        int gameState;
        unsigned long score;

        unsigned long currentTime;
        unsigned long oldTime;
        unsigned long lastPositionCheck;
        unsigned long lastBlink;
        unsigned long lastDifficultyIncrease;
        static const unsigned long moveDelayTime = 150;

        void reset();
        void update();
        void display();
        void getPlayerPosition();
        void shiftMatrixUp(int matrix[8][8]);
        void appendRow();

    public:
        FallGame(int joyXPin, int joyYPin, int joySwitchPin);
        void play();
};

#endif //FALL_GAME_H
