#pragma once

// Ptero
struct Ptero
{
    uint16_t x = Dimensions::width;
    uint8_t y = Dimensions::height - dinoHeight - random(6, 10);
    uint16_t spd = 2;
    float accel = 0.02;
    bool spawn;

    void update()
    {
        /*
        if(x > -pteroWidth && spawn)
            x -= spd;
        else if(spawn)
        {
            x = Dimensions::width + random(pteroWidth, 100);
            spawn = !spawn;
        }
        */
    }

    void draw()
    {
        /*
        if( (dino.frame % (scoreInterval / 2)) != 0)
            Sprites::drawSelfMasked(x, y, pteroImg, 0);
        else
            Sprites::drawSelfMasked(x, y, pteroImg, 2);

        */
    }
};
Ptero ptero;
