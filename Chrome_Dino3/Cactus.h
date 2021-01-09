#pragma once

// Cactus
struct Cactus
{
    float x = Arduboy2::width();
    uint16_t y = 43;
    float spd = 2;
    float accel = 0.02;
    bool spawnPtero = false;

    // Cactus Collision Points
    int16_t cactusRight;

    void update()
    {
        this->cactusRight = this->x + cactusWidth;

        if(spawnPtero)
        {
            if(this->x > -20)
                this->x -= spd;
            else
            {
                this->x = Arduboy2::width() + random( (cactusWidth * 2), (Arduboy2::width() * 2) );
                this->spd += this->accel;
            }
            
        }
        else
        {
            if(this->x > -20)
                this->x -= this->spd;
            else
            {
                this->x = Arduboy2::width() + random(cactusWidth, Arduboy2::width());
                this->spd += this->accel;
            }
        }

    }

    void draw()
    {
        Sprites::drawSelfMasked(this->x, this->y, cactusImg, 0);
    }
};