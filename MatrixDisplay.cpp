#include "Arduino.h"
#include "MatrixDisplay.h"

//Declaration of static members
volatile int MatrixDisplay::buttonState = 0;
int MatrixDisplay::lastButtonState = 0;
unsigned long MatrixDisplay::lastSwitch = 0;

//Display on the matrix an 8x8 frame
void MatrixDisplay::display(int nextFrame[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (ledState[i][j] == 0 && nextFrame[i][j] != 0) turnOnLed (i, j);
            if (ledState[i][j] != 0 && nextFrame[i][j] == 0) turnOffLed(i, j);
            ledState[i][j] = nextFrame[i][j];
        }
    }
}

//Turn on a specific led on the 8x8 matrix
void MatrixDisplay::turnOnLed(int row, int col) {
    row = 7 - row; // invert
    col = 7 - col; // invert
    //If coords outside the matrix, bring them in
    while (row < 0) row += 8;
    while (col < 0) col += 8;
    row = row % 8;
    col = col % 8;
    lc.setLed(0, row, col, true);
}

//Turn of a specific led on the 8x8 matrix
void MatrixDisplay::turnOffLed(int row, int col) {
    row = 7 - row; //invert
    col = 7 - col; //invert
    //If coords outside the matrix, bring them im
    while (row < 0) row += 8;
    while (col < 0) col += 8;
    row = row % 8;
    col = col % 8;
    lc.setLed(0, row, col, false);
}
