// Chrome Dino v3
// 22 October, 2020
// Ashlee J

#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "dino.h"
#include "dinoDuck.h"

#include "pterodactyl.h"

#include "cactus.h"
#include "cloud.h"

#include "TitleScreen.h"

// Screen Structure
struct Dimensions
{
    static constexpr uint8_t width = Arduboy2::width();
    static constexpr uint8_t height = Arduboy2::height();
};
Dimensions screen;

// Game Structure
struct Game
{
    int score, frame;
    bool soundMode;
};
Game game { 0, 0, true };

constexpr uint8_t groundHeight = 62;
constexpr uint8_t scoreInterval = 32;

// Dino Structure
struct Dino
{
    static constexpr uint8_t step = 10;
    int x, y, jumpVel, maxJump;
    bool autoJump;
};
Dino dino { 5, (screen.height - 2 - dinoHeight), 0, 8, false };

enum class DinoState
{
    Jumping,
    Falling,
    Running,
    Ducking,
};
DinoState dinoState = DinoState::Running; 

// Cactus Structure
struct Object
{
    float x, y, spd, accel;
};
Object ptero { screen.width, (screen.height - dinoHeight - random(6, 10)), 2, 0 };
Object cactus { screen.width, 43, 2, 0.02 };

Object cloud { screen.width + cloudWidth, 10, 1, 0 };

// Game State
enum class GameState
{
    Menu,
    Game,
    End
};
GameState gameState = GameState::Menu;

// Menu Cusor
enum class MenuCursor
{
    Start,
    AI,
    Sound
};
MenuCursor menuCursor = MenuCursor::Start;

// Reset
void reset()
{
    arduboy.initRandomSeed();

    dinoState = DinoState::Running;

    dino = { 5, (groundHeight - dinoHeight), 0, 8, dino.autoJump };
    ptero = { screen.width, (screen.height - dinoHeight - random(6, 10)), 2, 0.02 };
    cactus = { (screen.width + random(50, screen.width)), 43, 2, 0.02 };
    game = { 0, 0, game.soundMode };
}

void setup()
{
    arduboy.begin();
    arduboy.clear();
}

void loop()
{
    if(!arduboy.nextFrame())
        return;
    
    arduboy.pollButtons();

    arduboy.clear();

    switch(gameState)
    {
        case GameState::Menu:
            updateMenu();
            drawMenu();
            break;

        case GameState::Game:
            updateGame();
            drawGame();
            break;

        case GameState::End:
            updateEnd();
            drawEnd();
            break;
    }

    arduboy.display();
}

// Menu
void updateMenu()
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

void drawMenu()
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

    arduboy.setCursorY(46);

    if(game.soundMode)
    {
        arduboy.setCursorX(textToMiddle(8));
        arduboy.print(F("Sound:On"));
    }
    else
    {
        arduboy.setCursorX(textToMiddle(9));
        arduboy.print(F("Sound:Off"));
    }
    
}

void updateStart()
{
    if(arduboy.justPressed(DOWN_BUTTON) && menuCursor != MenuCursor::AI)
        menuCursor = MenuCursor::AI;

    arduboy.setCursor( (textToMiddle(8) / 2), 30);
    arduboy.print(F("A>"));
}

void updateAutoJump()
{
    if(arduboy.justPressed(A_BUTTON))
    {
        if(dino.autoJump)
            dino.autoJump = false;
        else
            dino.autoJump = true;
    }

    if(arduboy.justPressed(UP_BUTTON))
        menuCursor = MenuCursor::Start;
    else if(arduboy.justPressed(DOWN_BUTTON))
        menuCursor = MenuCursor::Sound;

    arduboy.setCursor( (textToMiddle(11) / 2), 38);
    arduboy.print(F("A>"));
}

void updateSound()
{
    if(arduboy.justPressed(A_BUTTON))
    {
        if(game.soundMode)
            game.soundMode = false;
        else
            game.soundMode = true;
    }

    if(arduboy.justPressed(UP_BUTTON))
        menuCursor = MenuCursor::AI;

    arduboy.setCursor( (textToMiddle(4) / 2), 46);
    arduboy.print(F("A>"));
}

void updateGame()
{
    updateDino();
    updatePtero();
    updateCactus();
    updateCloud();

    ++game.frame;

    if((game.frame % scoreInterval) != 0)
        ++game.score;
}

int textToMiddle(int charWidth)
{
    return (screen.width - (arduboy.getCharacterWidth(charWidth) + arduboy.getCharacterSpacing(charWidth - 1))) / 2;
}

void setCursorForScore(uint8_t x, uint8_t y)
{
    arduboy.setCursorY(y);

    if(game.score < 100)
        arduboy.setCursorX(textToMiddle(x + 0));
    else if(game.score < 1000)
        arduboy.setCursorX(textToMiddle(x + 1));
    else if(game.score < 10000)
        arduboy.setCursorX(textToMiddle(x + 2));
    else
        arduboy.setCursorX(textToMiddle(x + 3));
}

void drawGame()
{
    arduboy.drawLine(0, groundHeight, screen.width, groundHeight);

    setCursorForScore(2, 5);

    arduboy.print(game.score);

    drawPtero();
    drawCactus();
    drawCloud();
}

// Dino
void updateDino()
{
    switch(dinoState)
    {
        case DinoState::Running:
            dinoRunning();
            break;
        
        case DinoState::Jumping:
            dinoJumping();
            break;

        case DinoState::Falling:
            dinoFalling();
            break;

        case DinoState::Ducking:
            dinoDucking();
            break;
    }

    
    // Collision Detection
    if( (dino.y + dinoHeight) > cactus.y && (dino.x + dinoWidth - 4) > cactus.x && (dino.x + dinoWidth - 4) < cactus.x + cactusWidth )
    {
        gameState = GameState::End;

        if(game.soundMode)
            sound.tone(500, 100, 250, 200);
    }

    dino.y += dino.jumpVel;
}

void dinoRunning()
{
    dino.jumpVel = 0;
    dino.y = groundHeight - dinoHeight;

    if(dino.autoJump)
    {
        if(cactus.x - (dino.x + cactusWidth) < (cactus.spd * cactusHeight) && (cactus.x - dino.x) > 5)
            dinoState = DinoState::Jumping;
    }

    if(arduboy.justPressed(UP_BUTTON))
    {
        dinoState = DinoState::Jumping;

        if(game.soundMode)
            sound.tone(500, 50);
    }

    else if(arduboy.pressed(DOWN_BUTTON))
    {
        dinoState = DinoState::Ducking;

        if(game.soundMode)
            sound.tone(250, 50);
    }

    if( ((game.frame % dino.step) / 4 != 0))
        Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 1);
    else
        Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 2);
}

void dinoJumping()
{
    if(dino.y <= dino.maxJump)
        dinoState = DinoState::Falling;
    else
        dino.jumpVel = -2;

    Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 0);
}

void dinoFalling()
{
    if(dino.y >= (groundHeight - dinoHeight))
        dinoState = DinoState::Running;
    else
        dino.jumpVel = 2;

    Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 0);
}

void dinoDucking()
{
    if(arduboy.justReleased(DOWN_BUTTON))
        dinoState = DinoState::Running;

    dino.y = groundHeight - dinoDuckHeight;

    if((game.frame % dino.step) / 4 != 0)
        Sprites::drawSelfMasked(dino.x, dino.y, dinoDuckImg, 0);
    else
        Sprites::drawSelfMasked(dino.x, dino.y, dinoDuckImg, 1);
}

// Pterodactyl
void updatePtero()
{
    /*
    if(ptero.x > -pteroWidth)
        ptero.x -= ptero.spd;
    else
        ptero.x = screen.width + random(pteroWidth, 100);
    */
}

void drawPtero()
{
    /*
     if( (game.frame % (scoreInterval / 2)) != 0)
        Sprites::drawSelfMasked(ptero.x, ptero.y, pteroImg, 0);
    else
        Sprites::drawSelfMasked(ptero.x, ptero.y, pteroImg, 2);
    */
}

// Cactus
void updateCactus()
{
    if(cactus.x > -20)
        cactus.x -= cactus.spd;
    else
    {
        cactus.x = screen.width + random(cactusWidth, screen.width);
        cactus.spd += cactus.accel;
        
    }

}

void drawCactus()
{
    Sprites::drawSelfMasked(cactus.x, cactus.y, cactusImg, 0);
}

// Cloud
void updateCloud()
{
    if(cloud.x < -cloudWidth)
        cloud.x = screen.width + random(cloudWidth, 100);
    else
        cloud.x -= cloud.spd;
}

void drawCloud()
{
    Sprites::drawSelfMasked(cloud.x, cloud.y, cloudImg, 0);
}

// End
void updateEnd()
{
    if(arduboy.justPressed(A_BUTTON))
        gameState = GameState::Menu;
}

void drawEnd()
{
    arduboy.drawLine(0, 22, screen.width, 22);
    Sprites::drawSelfMasked( (screen.width - dinoDuckWidth) / 2, 14, dinoDuckImg, 2 );
    Sprites::drawSelfMasked( 20, (23 - cactusHeight), cactusImg, 0 );

    arduboy.setCursor( textToMiddle(10), 25);
    arduboy.print(F("GAME OVER!"));

    setCursorForScore(8, 35);

    arduboy.print(F("Score:"));
    arduboy.print(game.score);

    arduboy.setCursor( (screen.width - (arduboy.getCharacterWidth(12) + arduboy.getCharacterSpacing(11))) / 2 , 45);
    arduboy.print(F("A to Restart"));
}