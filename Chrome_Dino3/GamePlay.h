#pragma once

// GamePlay
struct GamePlay
{
    uint16_t score = 0;
    static constexpr uint8_t scoreInterval = 32;

    // SetCursorForScore
    void setCursorForScore(uint8_t x, uint8_t y)
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

    void update()
    {
        dino.update();
        ptero.update();
        cactus.update();
        cloud.update();

        ++dino.frame;

        if((dino.frame % scoreInterval) != 0)
            ++score;
    }

    void draw()
    {
        arduboy.drawLine(0, dino.groundHeight, Dimensions::width, dino.groundHeight);

        setCursorForScore(2, 5);

        arduboy.print(score);

        ptero.draw();
        cactus.draw();
        cloud.draw();
    }
};
GamePlay gamePlay;