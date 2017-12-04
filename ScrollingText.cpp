#include "Arduino.h"
#include "MatrixDisplay.h"
#include "ScrollingText.h"
#include <avr/pgmspace.h>

ScrollingText::ScrollingText() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            frame[i][j] = 0;
        }
    }
    currentTime = 0;
    oldTime = 0;
}

//Start scrolling a text, exit on button press or after displaying the message
int ScrollingText::scrollText(const char* text) {
    int i, j;
    for (i = 0; i < strlen(text); i++) {
        for (j = 0; j < 6; j++) {
            currentTime = millis();
            
            //Display the next column of a letter
            if (currentTime - oldTime > delayTime) {
                shiftMatrixLeft();
                appendLetterColumn(text[i], j);
                MatrixDisplay::getInstance() -> display(frame);
                oldTime = currentTime;
            }
            else j--;

            //If the button has changed its state since the last time, exit
            if(MatrixDisplay::buttonState != MatrixDisplay::lastButtonState ){
                MatrixDisplay::lastButtonState = MatrixDisplay::buttonState;
                return 1;
            }
        }
    }
    return 0;
}

//Add a column to the internal frame
void ScrollingText::appendLetterColumn(const char letter, int column) {
    byte andByte = 0;
    switch (column) {
        case 0: andByte = B10000000; break;
        case 1: andByte = B01000000; break;
        case 2: andByte = B00100000; break;
        case 3: andByte = B00010000; break;
        case 4: andByte = B00001000; break;
        case 5: andByte = B00000100; break;
        case 6: andByte = B00000010; break;
        case 7: andByte = B00000001; break;
        default: return;
    }
    byte letterRow;
    for (int i = 0; i < 8; i++) {
        letterRow = getLetterByte(letter, i);
        frame[i][7] = letterRow & andByte;
    }
}

/*
 * Returns the byte corresponding to a letter/number/symbol
 * read from the flash memory.
 */
byte ScrollingText::getLetterByte(const char letter, int row) {
    int letterNum;
    if (letter == ' ') return B00000000;
    if (letter >= 'a' && letter <= 'z') {
        letterNum = letter - 'a';
        return pgm_read_byte((byte*)pgm_read_ptr(&(letterTable[letterNum])) + row);
    }
    if (letter >= 'A' && letter <= 'Z') {
        letterNum = letter - 'A';
        return pgm_read_byte((byte*)pgm_read_ptr(&(letterTable[letterNum])) + row);
    }
    if (letter >= '0' && letter <= '9') {
        letterNum = letter - '0';
        return pgm_read_byte((byte*)pgm_read_ptr(&(numberTable[letterNum])) + row);
    }
    switch (letter) {
        case '?': return pgm_read_byte(spriteQuestion + row);
        case '!': return pgm_read_byte(spriteExclamation + row);
        case '_': return pgm_read_byte(spriteUnderscore + row);
        case '#': return pgm_read_byte(spriteHashtag + row);
        case '$': return pgm_read_byte(spriteDollar + row);
        case '(': return pgm_read_byte(spriteOpenBracket + row);
        case ')': return pgm_read_byte(spriteCloseBracket + row);
        case '+': return pgm_read_byte(spritePlus + row);
        case '-': return pgm_read_byte(spriteMinus + row);
        case '/': return pgm_read_byte(spriteSlash + row);
        case ':': return pgm_read_byte(spriteColon + row);
        case ';': return pgm_read_byte(spriteSemicolon + row);
        case '=': return pgm_read_byte(spriteEqual + row);
    }
    return B00000000;
}

//Shift the internal matrix to the left once
void ScrollingText::shiftMatrixLeft() {
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 7; j++) {
            frame[i][j] =  frame [i][j + 1];
        }
    }
    for (i = 0; i < 8; i++) {
        frame[i][7] = 0;
    }
}
