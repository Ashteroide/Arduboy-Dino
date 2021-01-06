#pragma once

// Ptero
struct Ptero
{
    static constexpr uint8_t step = 10;
    int16_t x = Arduboy2::width();
    int8_t y = Arduboy2::height() - dinoHeight - random(6, 10);
    uint8_t cactusDistance;
    uint16_t frame = 0;

    void update()
    {
        if(cactus.x < -20)
            this->cactusDistance = random(50, 80);
        if(cactus.spawnPtero)
            this->x = cactus.x + this->cactusDistance;

        ++this->frame;
    }

    void draw()
    {
        if((this->frame % this->step) / 4 != 0)
            Sprites::drawSelfMasked(this->x, this->y, pteroImg, 0);
        else
            Sprites::drawSelfMasked(this->x, this->y, pteroImg, 2);
    }
};
Ptero ptero;
