#pragma once

// Dino
struct Dino
{
    static constexpr uint8_t animationStep = 40;
    static constexpr uint8_t maxJumpHeight = 8;
    static constexpr uint8_t groundHeight = 62;
    static constexpr uint8_t x = 5;
    int16_t y = (Arduboy2::height() - 2 - dinoHeight);
    uint16_t dinoFrameCounter = 0;
    int8_t jumpVelocity = 0;
    bool autoJump = false;

    void update()
    {
        switch(dinoState)
        {
            case DinoState::updateRunningState:
                updateRunningState();
                break;
            
            case DinoState::updateJumpingState:
                updateJumpingState();
                break;

            case DinoState::updateFallingState:
                updateFallingState();
                break;

            case DinoState::updateDuckingState:
                updateDuckingState();
                break;
        }
        
        // Collision Detection
        if((this->y + dinoHeight) > cactus.y && (this->x + dinoWidth - 4) > cactus.x && (this->x + dinoWidth - 4) < cactus.x + cactusWidth)
        {
            gameState = GameState::End;

            sound.tone(500, 100, 250, 200);
        }

        this->y += this->jumpVelocity;

        ++this->dinoFrameCounter;
    }

    void updateRunningState()
    {
        this->jumpVelocity = 0;
        this->y = this->groundHeight - dinoHeight;

        if(this->autoJump)
        {
            if(cactus.x - (this->x + cactusWidth) < (cactus.spd * cactusHeight) && (cactus.x - this->x) > 5)
                dinoState = DinoState::updateJumpingState;
        }

        if(arduboy.justPressed(UP_BUTTON))
        {
            dinoState = DinoState::updateJumpingState;

            sound.tone(500, 50);
        }

        else if(arduboy.pressed(DOWN_BUTTON))
        {
            dinoState = DinoState::updateDuckingState;

            sound.tone(250, 50);
        }

        if((this->dinoFrameCounter % this->animationStep) != 0)
            Sprites::drawSelfMasked(this->x, this->y, dinoImg, 2);
        else
            Sprites::drawSelfMasked(this->x, this->y, dinoImg, 1);
    }

    void updateJumpingState()
    {
        if(this->y <= this->maxJumpHeight)
            dinoState = DinoState::updateFallingState;
        else
            this->jumpVelocity = -2;

        Sprites::drawSelfMasked(this->x, this->y, dinoImg, 0);
    }

    void updateFallingState()
    {
        if(this->y >= (this->groundHeight - dinoHeight))
            dinoState = DinoState::updateRunningState;
        else
            this->jumpVelocity = 2;

        Sprites::drawSelfMasked(this->x, this->y, dinoImg, 0);
    }

    void updateDuckingState()
    {
        if(arduboy.justReleased(DOWN_BUTTON))
            dinoState = DinoState::updateRunningState;

        this->y = this->groundHeight - dinoDuckHeight;

        if((this->dinoFrameCounter % this->animationStep) != 0)
            Sprites::drawSelfMasked(this->x, this->y, dinoDuckImg, 1);
        else
            Sprites::drawSelfMasked(this->x, this->y, dinoDuckImg, 0);
    }
};
Dino dino;