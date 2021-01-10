#pragma once

// Cactus
struct Cactus
{
    float x = Arduboy2::width();
    uint16_t y = 43;
    float spd = 2;
    float accel = 0.02;

    // Cactus Collision Points
    int16_t cactusRight;

    void update()
    {
        this->cactusRight = this->x + cactusWidth;
    }

    void draw()
    {
        Sprites::drawSelfMasked(this->x, this->y, cactusImg, 0);
    }
};