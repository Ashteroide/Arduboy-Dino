#pragma once

// MenuState
struct MenuState
{
    void reset()
    {
        cactus.x = Arduboy2::width();
        cactus.spd = 2;
        cactus.accel = 0.02;

        cactus.spawnPtero = false;
        ptero.x = Arduboy2::width();

        cloud.x = Arduboy2::width() + cloudWidth;

        arduboy.initRandomSeed();

        dinoState = DinoState::updateRunningState;

        gamePlay.score = 0;

        data.load();
    }

    void update()
    {
        if(arduboy.justPressed(A_BUTTON) && menuCursor == MenuCursor::Start)
        {
            gameState = GameState::Game;
            reset();
        }

        switch(menuCursor)
        {
            case MenuCursor::Start:
                updateStart();
                break;

            case MenuCursor::AI:
                updateAutoJump();
                break;

            case MenuCursor::Sound:
                updateSound();
                break;
        }
    }

    void draw()
    {
        Sprites::drawSelfMasked(0, 0, TitleScreenImg, 0);

        arduboy.setCursor(textToMiddle(10), 30);
        arduboy.print(F("Start Game"));

        arduboy.setCursorY(38);

        if(dino.autoJump)
        {
            arduboy.setCursorX(textToMiddle(11));
            arduboy.print(F("AutoJump:On"));
        }
        else
        {
            arduboy.setCursorX(textToMiddle(12));
            arduboy.print(F("AutoJump:Off"));
        }

        arduboy.setCursorX(arduboy.audio.enabled() ? textToMiddle(8) : textToMiddle(9));
        arduboy.setCursorY(46);

        arduboy.print(F("Sound:"));
        arduboy.print(arduboy.audio.enabled() ? F("On") : F("Off"));
        
    }

    void updateSound()
    {
        if(arduboy.justPressed(A_BUTTON))
            arduboy.audio.toggle();

        if(arduboy.justPressed(UP_BUTTON))
            menuCursor = MenuCursor::AI;

        arduboy.setCursor((textToMiddle(4) / 2), 46);
        arduboy.print(F("A>"));
    }

    void updateAutoJump()
    {
        if(arduboy.justPressed(A_BUTTON))
            dino.autoJump = !dino.autoJump;

        if(arduboy.justPressed(UP_BUTTON))
            menuCursor = MenuCursor::Start;
        else if(arduboy.justPressed(DOWN_BUTTON))
            menuCursor = MenuCursor::Sound;

        arduboy.setCursor((textToMiddle(11) / 2), 38);
        arduboy.print(F("A>"));
    }

    void updateStart()
    {
        if(arduboy.justPressed(DOWN_BUTTON) && menuCursor != MenuCursor::AI)
            menuCursor = MenuCursor::AI;

        arduboy.setCursor((textToMiddle(8) / 2), 30);
        arduboy.print(F("A>"));
    }
};