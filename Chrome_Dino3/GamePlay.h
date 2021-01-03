#pragma once

// GamePlay
struct GamePlay
{
    uint16_t score = 0;
    static constexpr uint8_t scoreInterval = 32;

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

        setCursorForScore(2, 5, score);

        arduboy.print(score);

        ptero.draw();
        cactus.draw();
        cloud.draw();
    }
};
GamePlay gamePlay;