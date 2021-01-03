#pragma once

// Cloud
struct Cloud
{
    uint16_t x = Dimensions::width + cloudWidth;
    uint8_t y = 10;
    uint8_t spd = 1;

    void update()
    {
        if(x < -cloudWidth)
            x = Dimensions::width + random(cloudWidth, 100);
        else
            x -= spd;
    }

    void draw()
    {
        Sprites::drawSelfMasked(x, y, cloudImg, 0);
    }
};
Cloud cloud;