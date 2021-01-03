#pragma once

// Cactus
struct Cactus
{
    float x = Dimensions::width;
    uint16_t y = 43;
    float spd = 2;
    float accel = 0.02;

    void update()
    {
        if(x > -20)
            x -= spd;
        else
        {
            x = Dimensions::width + random(cactusWidth, Dimensions::width);
            spd += accel;
            ptero.spawn = true;
        }

    }

    void draw()
    {
        Sprites::drawSelfMasked(x, y, cactusImg, 0);
    }
};
Cactus cactus;