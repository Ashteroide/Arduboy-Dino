#pragma once

// SetCursorForScore
void setCursorForScore(uint8_t x, uint8_t y, uint8_t score)
{
    arduboy.setCursorY(y);

    if(score < 100)
        arduboy.setCursorX(textToMiddle(x + 0));
    else if(score < 1000)
        arduboy.setCursorX(textToMiddle(x + 1));
    else if(score < 10000)
        arduboy.setCursorX(textToMiddle(x + 2));
    else
        arduboy.setCursorX(textToMiddle(x + 3));
}