#pragma once

#include "Cactus.h"
Cactus cactus;
#include "Ptero.h"
Ptero ptero;
#include "Cloud.h"
Cloud cloud;
#include "Dino.h"
Dino dino;

// GamePlayState
struct GamePlayState
{
    static constexpr uint8_t scoreInterval = 32;
    uint16_t score = 0;

    void update()
    {
        dino.update();
        ptero.update();
        cactus.update();
        cloud.update();

        if(arduboy.everyXFrames(10))
            ++this->score;
        
        if(this->score > 250 && cactus.x > 128)
            cactus.spawnPtero = true;
    }

    void draw()
    {
        arduboy.drawLine(0, dino.groundHeight, Arduboy2::width(), dino.groundHeight);

        setCursorForScore(2, 5, this->score);

        arduboy.print(this->score);

        ptero.draw();
        cactus.draw();
        cloud.draw();
    }
};