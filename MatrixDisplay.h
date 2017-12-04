#ifndef MATRIX_DISPLAY_H
#define MATRIX_DISPLAY_H

#include "Arduino.h"
#include "LedControl.h"

/*
    Singleton class that handles all the required actions to display
    information on the matrix.
    Access is done via getInstance().
*/
class MatrixDisplay {
    private:
        int ledState [8][8];
        LedControl lc = LedControl(12, 11, 10, 1);
        MatrixDisplay() {
            lc.shutdown(0, false);
            /* Set the brightness to a medium values */
            lc.setIntensity(0, 8);
            /* and clear the display */
            lc.clearDisplay(0);

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    ledState[i][j] = 0;
                }
            }
        }
    public:
        volatile static int buttonState;
        static int lastButtonState;
        static unsigned long lastSwitch;
        
        static MatrixDisplay* getInstance() {
            static MatrixDisplay matrix;
            return &matrix;
        }
        void display(int nextFrame[8][8]);
        void turnOnLed (int row, int col);
        void turnOffLed(int row, int col);
};

#endif //MATRIX_DISPLAY_H
