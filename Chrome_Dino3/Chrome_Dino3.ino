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
    int width, height;
};
Dimensions screen { Arduboy2::width(), Arduboy2::height() };
Dimensions font { 6, 8 };

// Game Structure
struct Game
{
    int score, frame;
};
Game game { 0, 0 };

constexpr uint8_t groundHeight = 62;
constexpr uint8_t scoreInterval = 32;

// Dino Structure
struct Dino
{
    int x, y, jumpVel, maxJump, step;
};
Dino dino { 5, (screen.height - 2 - dinoHeight), 0, 10, 8 };

enum class DinoState
{
    Jumping,
    Falling,
    Running,
    Ducking,
};
DinoState dinoState = DinoState::Running; 

// Pterodactyl Structure
struct Pterodactyl
{
    float x, y, spd;
    bool spawn;
};
Pterodactyl ptero { screen.width, (screen.height - dinoHeight - random(6, 10)), true };

// Cactus Structure
struct Object
{
    float x, y, spd;
};
Object cactus { screen.width, 43, 2 };
Object cloud { screen.width + cloudWidth, 10, 1 };

// Game State
enum class GameState
{
    Menu,
    Game,
    End
};
GameState gameState = GameState::Menu;

// Reset
void reset()
{
    arduboy.initRandomSeed();

    dinoState = DinoState::Running;

    dino = { 5, (groundHeight - dinoHeight), 0, 10, 8 };
    ptero = { screen.width, (screen.height - dinoHeight - random(6, 10)), true };
    cactus = { (screen.width + random(50, screen.width)), 43, 2 };
    game = { 0, 0 };
}

void setup()
{
    arduboy.begin();
    arduboy.clear();
}

void loop()
{
    if(!arduboy.nextFrame()) return;
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
    if(arduboy.justPressed(A_BUTTON))
    {
        reset();
        gameState = GameState::Game;
    }
}

void drawMenu()
{
    Sprites::drawSelfMasked(0, 0, TitleScreenImg, 0);
    
    arduboy.setCursor(34, 30);
    arduboy.println(F("A to Start"));
}

void updateGame()
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

    updateDino();
    updatePtero();
    updateCactus();
    updateCloud();

    ++game.frame;

    if((game.frame % scoreInterval) != 0)
        ++game.score;
}

void drawGame()
{
    arduboy.drawLine(0, groundHeight, screen.width, groundHeight);

    arduboy.setCursorY(5);
    
    if(game.score < 100)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(2) + arduboy.getCharacterSpacing(1))) / 2);
    if(game.score >= 100 && game.score < 1000)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(3) + arduboy.getCharacterSpacing(2))) / 2);
    if(game.score >= 1000 && game.score < 10000)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(4) + arduboy.getCharacterSpacing(3))) / 2 );
    if(game.score >= 10000)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(5) + arduboy.getCharacterSpacing(4))) / 2 );

    arduboy.print(game.score);

    drawPtero();
    drawCactus();
    drawCloud();
}

// Dino
void updateDino()
{
    // Collision Detection
    if( (dino.y + dinoHeight) > cactus.y && (dino.x + dinoWidth - 4) > cactus.x && (dino.x + dinoWidth - 4) < cactus.x + cactusWidth )
    {
        gameState = GameState::End;

        sound.tone(500, 100, 250, 200);
    }

    dino.y += dino.jumpVel;
}

void dinoRunning()
{
    dino.jumpVel = 0;
    dino.y = groundHeight - dinoHeight;

    if(arduboy.justPressed(UP_BUTTON))
    {
        dinoState = DinoState::Jumping;
        sound.tone(500, 50);
    }

    else if(arduboy.pressed(DOWN_BUTTON))
    {
        dinoState = DinoState::Ducking;
        sound.tone(250, 50);
    }

    if( ((game.frame % dino.step) / 4 != 0))
        Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 1);
    else if( !((game.frame % dino.step) / 4 != 0))
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
        cactus.x = screen.width + random(cactusWidth, screen.width);
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
    Sprites::drawSelfMasked( (screen.width / 4) * 3, (23 - cactusHeight), cactusImg, 0 );

    arduboy.setCursor( (screen.width - (arduboy.getCharacterWidth(10) + arduboy.getCharacterSpacing(9))) / 2, 25);
    arduboy.print(F("GAME OVER!"));

    arduboy.setCursorY(35);

    if(game.score < 100)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(8) + arduboy.getCharacterSpacing(7))) / 2 );
    if(game.score >= 100 && game.score < 1000)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(9) + arduboy.getCharacterSpacing(8))) / 2 );
    if(game.score >= 1000 && game.score < 10000)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(10) + arduboy.getCharacterSpacing(9))) / 2 );
    if(game.score >= 10000)
        arduboy.setCursorX( (screen.width - (arduboy.getCharacterWidth(12) + arduboy.getCharacterSpacing(11))) / 2 );

    arduboy.print(F("Score:"));
    arduboy.print(game.score);

    arduboy.setCursor( (screen.width - (arduboy.getCharacterWidth(12) + arduboy.getCharacterSpacing(11))) / 2 , 45);
    arduboy.print(F("A to Restart"));
}