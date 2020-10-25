// Chrome Dino v3
// 22 October, 2020
// Ashlee J

#include <Arduboy2.h>

Arduboy2 arduboy;

#include "dino.h"
#include "dinoDuck.h"

#include "pterodactyl.h"

#include "cactus.h"
#include "cloud.h"

#include "TitleScreen.h"

// Screen Structure
struct Screen
{
    int width, height;
};
Screen screen { Arduboy2::width(), Arduboy2::height() };

// Game Structure
struct Game
{
    int score, frame;
};
Game game { 0, 0 };

const int groundHeight = 62;
const int maxFrame = 32;

struct Font
{
    int width, height;
};
Font font { 6, 8 };

// Dino Structure
struct Dino
{
    float x, y, jumpVel, maxJump;
    bool jump, fall, duck;
};
Dino dino { 5, (screen.height - 2 - dinoHeight), 0, 10, false, false, false };

// Pterodactyl Structure
struct Pterodactyl
{
    float x, y, spd;
    bool spawn;
};
Pterodactyl ptero { screen.width, (screen.height - dinoHeight - random(6, 10)) };

// Cactus Structure
struct Object
{
    float x, y, spd;
};
Object cactus { screen.width, 43, 2 };
Object cloud { screen.width, 43, 1 };

// Game State
enum class GameState
{
    Menu,
    Game,
    End,
};
GameState gameState = GameState::Menu;

// Reset
void reset()
{
    arduboy.initRandomSeed();

    dino = { 5, (groundHeight - dinoHeight), 0, 10, false, false, false };
    ptero = { screen.width, (screen.height - dinoHeight - random(6, 10)) };
    cactus = { (screen.width + random(80, 120)), 43, 2 };
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
    reset();
    if(arduboy.justPressed(A_BUTTON)) gameState = GameState::Game;
}

void drawMenu()
{
    Sprites::drawSelfMasked(0, 0, TitleScreenImg, 0);
    
    arduboy.setCursor(34, 30);
    arduboy.println(F("A to Start"));
}

void updateGame()
{
    updateDino();
    updatePtero();
    updateCactus();
    updateCloud();

    ++game.frame;

    if((game.frame % maxFrame) != 0) ++game.score;
}

void drawGame()
{
    arduboy.drawLine(0, groundHeight, screen.width, groundHeight);
    
    if(game.score < 100)
        arduboy.setCursor( ((screen.width / 2) - font.width), 5);
    else if(game.score < 1000)
        arduboy.setCursor( ((screen.width / 2) - ((font.width * 3) / 2)), 5);
    else if(game.score < 10000)
        arduboy.setCursor( ((screen.width / 2) - (font.width * 2)), 5);
    else
        arduboy.setCursor( (screen.width / 2) - (font.width / 2), 5);

    arduboy.print(game.score);

    drawDino();
    drawPtero();
    drawCactus();
    drawCloud();
}

// Dino
void updateDino()
{
    // If dino.jumpVel is other than 0 the Dino is falling
    if((dino.jumpVel >= 1) && (dino.jumpVel <= 2))
        dino.fall = true;
    else
        dino.fall = false;

    // If dino.y is on ground line dino.jumpVel is 0
    if(dino.y >= (groundHeight - dinoHeight))
    {
        dino.jumpVel = 0;
        dino.fall = false;
        dino.jump = false;
    }

    if(arduboy.justPressed(UP_BUTTON) && !dino.jump)
    {
        dino.jumpVel = -2;
        dino.jump = true;
        tone(500, 100);
    }

    // If dino.y is smaller than dino.maxJump move Dino down
    if(dino.y <= dino.maxJump)
        dino.jumpVel = 2;

    // If dino.y = (dino.maxJump + 6) the dino.jumpVel is 1 to slow speed
    if(dino.y == (dino.maxJump + 6) && dino.jump && !dino.fall)
        dino.jumpVel = -1;

    // Collision Detection
    // if( (dino.y + dinoHeight) > cactus.y && (dino.x + dinoWidth - 4) > cactus.x && (dino.x + dinoWidth - 4) < cactus.x + cactusWidth); gameState = GameState::End;
    if( (dino.y + dinoHeight) > cactus.y && (dino.x + dinoWidth - 4) > cactus.x && (dino.x + dinoWidth - 4) < cactus.x + cactusWidth )
        gameState = GameState::End;

    dino.y += dino.jumpVel;
}

void drawDino()
{
    if( ((game.frame % 8) / 4) && !dino.jump && !dino.duck) Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 1);
    else if( !((game.frame % 8) / 4) && !dino.jump && !dino.duck) Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 2);
    else Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 0);

    if(dino.jump && !dino.duck) Sprites::drawSelfMasked(dino.x, dino.y, dinoImg, 0);
}

// Pterodactyl
void updatePtero()
{
}

void drawPtero()
{
}

// Cactus
void updateCactus()
{
    if(cactus.x > -20) cactus.x -= cactus.spd;
    else
    {
        cactus.x = screen.width + random(cactusWidth, 100);
        Serial.println(cactus.x);
    }
}

void drawCactus()
{
    Sprites::drawSelfMasked(cactus.x, cactus.y, cactusImg, 0);
}

// Cloud
void updateCloud()
{
}

void drawCloud()
{
    Sprites::drawSelfMasked(cloud.x, cloud.y, cloudImg, 0);
}

// End
void updateEnd()
{
    if(arduboy.justPressed(A_BUTTON)) gameState = GameState::Menu;
}

void drawEnd()
{
    arduboy.setCursor(34, 22);
    arduboy.print(F("GAME OVER!"));

    if(game.score < 100) arduboy.setCursor(40, 32);
    if(game.score >= 100 && game.score < 1000) arduboy.setCursor(37, 32);
    if(game.score >= 1000 && game.score < 10000) arduboy.setCursor(34, 32);
    if(game.score >= 10000) arduboy.setCursor(31, 32);

    arduboy.print(F("Score:"));
    arduboy.print(game.score);

    arduboy.setCursor(11, 42);
    arduboy.print(F("Press A to Restart"));
}