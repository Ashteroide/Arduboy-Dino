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
Screen screen = { Arduboy2::width(), Arduboy2::height() };

// Game Structure
struct Game
{
    int frame, maxFrame;
    uint16_t score;
};
Game game = { 0, 32, 0 };

struct Font
{
    int width, height;
};
Font font = { 6, 8 };

struct Velocity
{
    const float spd[4];
};
Velocity velo = { 0, 0.5, 1, 2 };

// Dino Structure
struct Dino
{
    float x, y, vel, maxJump;
    bool jump, fall, duck;
};
Dino dino = { 5, (screen.height - 2 - dinoHeight), 0, 10, false, false, false };

// Pterodactyl Structure
struct Pterodactyl
{
    float x, y, spd;
    bool spawn;
};
Pterodactyl ptero = { screen.width, (screen.height - dinoHeight - random(6, 10)) };

// Cactus Structure
struct Cactus
{
    float x, y, spd;
};
Cactus cactus = { (screen.width + random(80, 120)), 43, 2 };

struct Cloud
{
    int x, y, spd;
};
Cloud cloud = { screen.width + random(0, 60), 43, 1 };

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

    dino = { 5, (screen.height - 2 - dinoHeight), 0, 10, false, false, false };
    ptero = { screen.width, (screen.height - dinoHeight - random(6, 10)) };
    cactus = { (screen.width + random(80, 120)), 43, 2 };
    game = { 0, 32, 0 };
}

void setup()
{
    arduboy.begin();
    arduboy.clear();

    reset();
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
    if(game.frame % game.maxFrame) game.score += 1;
}

void drawGame()
{
    arduboy.drawLine(0, (screen.height - 2), screen.width, (screen.height - 2));
    
    if(game.score >= 10 && game.score < 100) arduboy.setCursor( ((screen.width / 2) - font.width), 5);
    else if(game.score >= 100 && game.score < 1000) arduboy.setCursor( ((screen.width / 2) - ((font.width * 3) / 2)), 5);
    else if(game.score >= 1000) arduboy.setCursor( ((screen.width / 2) - (font.width * 2)), 5);
    else arduboy.setCursor( (screen.width / 2) - (font.width / 2), 5);

    arduboy.print(game.score);

    drawDino();
    drawPtero();
    drawCactus();
    drawCloud();
}

// Dino
void updateDino()
{
    // If dino.vel is other than 0 the Dino is falling
    if((dino.vel >= velo.spd[1]) && (dino.vel <= velo.spd[3])) dino.fall = true;
    else dino.fall = false;

    // If Dino is equal to ground line then jumping is false
    if(dino.y >= (screen.height - 2 - dinoHeight)) dino.jump = false;
    if(arduboy.justPressed(UP_BUTTON) && !dino.jump)
    {
        dino.vel = -velo.spd[3];
        dino.jump = true;
        tone(500, 100);
    }

    // If dino.y is smaller than dino.maxJump move Dino down
    if(dino.y <= dino.maxJump) dino.vel = velo.spd[3];

    // If dino.y = (dino.maxJump + 6) the dino.vel is 1 to slow speed
    if(dino.y == (dino.maxJump + 6) && dino.jump && !dino.fall) dino.vel = -velo.spd[2];

    // If dino.y is on ground line dino.vel is 0
    if(dino.y >= (screen.height - 2 - dinoHeight) && !dino.jump)
    {
        dino.vel = velo.spd[0];
        dino.fall = false;
    }

    // Collision Detection
    // if( (dino.y + dinoHeight) > cactus.y && (dino.x + dinoWidth - 4) > cactus.x && (dino.x + dinoWidth - 4) < cactus.x + cactusWidth); gameState = GameState::End;
    if( (dino.y + dinoHeight) > cactus.y && (dino.x + dinoWidth - 4) > cactus.x && (dino.x + dinoWidth - 4) < cactus.x + cactusWidth ) gameState = GameState::End;

    dino.y += dino.vel;
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