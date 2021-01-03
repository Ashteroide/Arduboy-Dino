#pragma once

// Dino
struct Dino
{
    static constexpr uint8_t step = 10;
    uint16_t frame = 0;
    static constexpr uint8_t maxJump = 8;
    static constexpr uint8_t groundHeight = 62;
    static constexpr uint8_t x = 5;
    int16_t y = (Dimensions::height - 2 - dinoHeight);
    int jumpVel = 0;
    bool autoJump = false;
    bool soundMode = false;

    void update()
    {
        switch(dinoState)
        {
            case DinoState::Running:
                Running();
                break;
            
            case DinoState::Jumping:
                Jumping();
                break;

            case DinoState::Falling:
                Falling();
                break;

            case DinoState::Ducking:
                Ducking();
                break;
        }
        
        // Collision Detection
        if( (y + dinoHeight) > cactus.y && (x + dinoWidth - 4) > cactus.x && (x + dinoWidth - 4) < cactus.x + cactusWidth )
        {
            gameState = ChromeDino::End;

            if(soundMode)
                sound.tone(500, 100, 250, 200);
        }

        y += jumpVel;
    }

    void Running()
    {
        jumpVel = 0;
        y = groundHeight - dinoHeight;

        if(autoJump)
        {
            if(cactus.x - (x + cactusWidth) < (cactus.spd * cactusHeight) && (cactus.x - x) > 5)
                dinoState = DinoState::Jumping;
        }

        if(arduboy.justPressed(UP_BUTTON))
        {
            dinoState = DinoState::Jumping;

            if(soundMode)
                sound.tone(500, 50);
        }

        else if(arduboy.pressed(DOWN_BUTTON))
        {
            dinoState = DinoState::Ducking;

            if(soundMode)
                sound.tone(250, 50);
        }

        if( ((frame % step) / 4 != 0))
            Sprites::drawSelfMasked(x, y, dinoImg, 1);
        else
            Sprites::drawSelfMasked(x, y, dinoImg, 2);
    }

    void Jumping()
    {
        if(y <= maxJump)
            dinoState = DinoState::Falling;
        else
            jumpVel = -2;

        Sprites::drawSelfMasked(x, y, dinoImg, 0);
    }

    void Falling()
    {
        if(y >= (groundHeight - dinoHeight))
            dinoState = DinoState::Running;
        else
            jumpVel = 2;

        Sprites::drawSelfMasked(x, y, dinoImg, 0);
    }

    void Ducking()
    {
        if(arduboy.justReleased(DOWN_BUTTON))
            dinoState = DinoState::Running;

        y = groundHeight - dinoDuckHeight;

        if((frame % step) / 4 != 0)
            Sprites::drawSelfMasked(x, y, dinoDuckImg, 0);
        else
            Sprites::drawSelfMasked(x, y, dinoDuckImg, 1);
    }
};
Dino dino;
