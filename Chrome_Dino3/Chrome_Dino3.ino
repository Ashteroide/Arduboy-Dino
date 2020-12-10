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
Dimensions;

// Game Structure
struct Game
{
    uint16_t score, frame;
    bool soundMode;
};
Game game { 0, 0, false };

constexpr uint8_t groundHeight = 62;
constexpr uint8_t scoreInterval = 32;

struct HighScoreEntry
{
    uint16_t score;
    char name[3];
};

// High Score Structure
struct SaveData
{
    HighScoreEntry highscores[3];
};
SaveData saveData;

constexpr uint16_t saveDataLocation = 272;

uint8_t letter[3], nameCursor, place;
uint16_t nameCursorPosX;

// Dino Structure
struct Dino
{
    static constexpr uint8_t step = 10;
    int x, y, jumpVel, maxJump;
    bool autoJump;
};
Dino dino { 5, (Dimensions::height - 2 - dinoHeight), 0, 8, false };

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
Object ptero { Dimensions::width, (Dimensions::height - dinoHeight - random(6, 10)), 2, 0 };
bool pteroSpawn;

Object cactus { Dimensions::width, 43, 2, 0.02 };

Object cloud { Dimensions::width + cloudWidth, 10, 1, 0 };

// Game State
enum class GameState
{
    Menu,
    Game,
    End,
    HighScore,
    Name
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

bool nameEntered = false;

// Reset
void reset()
{
    arduboy.initRandomSeed();

    dinoState = DinoState::Running;

    dino = { 5, (groundHeight - dinoHeight), 0, 8, dino.autoJump };
    ptero = { Dimensions::width, (Dimensions::height - dinoHeight - random(6, 10)), 2, 0.02 };
    pteroSpawn = true;
    cactus = { (Dimensions::width + random(50, Dimensions::width)), 43, 2, 0.02 };
    game = { 0, 0, game.soundMode };
    nameEntered = false;

    loadSaveData();
}

void saveSaveData()
{
    EEPROM.put(saveDataLocation, saveData);
}

void loadSaveData()
{
    EEPROM.get(saveDataLocation, saveData);
}

void clearSaveData()
{
	auto data = reinterpret_cast<uint8_t *>(&saveData);
	for(size_t index = 0; index < sizeof(saveData); ++index)
		data[index] = 0;
	
	saveSaveData();
}

void setup()
{
    arduboy.begin();
    arduboy.clear();

    // clearSaveData();
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
        
        case GameState::HighScore:
            updateHighScores();
            drawHighscores();
            break;

        case GameState::Name:
            updateName();
            drawName();
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
        dino.autoJump = !dino.autoJump;

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
        game.soundMode = !game.soundMode;

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
    return (Dimensions::width - (arduboy.getCharacterWidth(charWidth) + arduboy.getCharacterSpacing(charWidth - 1))) / 2;
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
    arduboy.drawLine(0, groundHeight, Dimensions::width, groundHeight);

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
    if(ptero.x > -pteroWidth && pteroSpawn)
        ptero.x -= ptero.spd;
    else if(pteroSpawn)
    {
        ptero.x = Dimensions::width + random(pteroWidth, 100);
        pteroSpawn = !pteroSpawn;
    }
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
        cactus.x = Dimensions::width + random(cactusWidth, Dimensions::width);
        cactus.spd += cactus.accel;
        pteroSpawn = true;
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
        cloud.x = Dimensions::width + random(cloudWidth, 100);
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
    if(!dino.autoJump)
    {
        if(game.score > saveData.highscores[0].score)
        {
            saveData.highscores[2].score = saveData.highscores[1].score;
            saveData.highscores[1].score = saveData.highscores[0].score;
            saveData.highscores[0].score = game.score;

            if(!nameEntered)
            {
                place = 1;
                gameState = GameState::Name;
                nameEntered = true;
            }
        }
        else if(game.score > saveData.highscores[1].score && game.score < saveData.highscores[0].score)
        {
            saveData.highscores[2].score = saveData.highscores[1].score;
            saveData.highscores[1].score = game.score;

            if(!nameEntered)
            {
                place = 2;
                gameState = GameState::Name;
                nameEntered = true;
            }
        }
        else if(game.score > saveData.highscores[2].score && game.score < saveData.highscores[1].score)
        {
            saveData.highscores[2].score = game.score;

            if(!nameEntered)
            {
                place = 3;
                gameState = GameState::Name;
                nameEntered = true;
            }
        }
    }

    if(arduboy.justPressed(A_BUTTON))
    {
        saveSaveData();
        gameState = GameState::Menu;
    }
    if(arduboy.justPressed(B_BUTTON))
        gameState = GameState::HighScore;
}

void drawEnd()
{
    arduboy.drawLine(0, 20, Dimensions::width, 20);
    Sprites::drawSelfMasked( (Dimensions::width - dinoDuckWidth) / 2, 12, dinoDuckImg, 2 );
    Sprites::drawSelfMasked( 20, (21 - cactusHeight), cactusImg, 0 );

    arduboy.setCursor( textToMiddle(10), 22);
    arduboy.print(F("GAME OVER!"));

    setCursorForScore(8, 32);

    arduboy.print(F("Score:"));
    arduboy.print(game.score);

    arduboy.setCursor( textToMiddle(9), 42);
    arduboy.print(F("A:Restart"));

    arduboy.setCursor( textToMiddle(12), 52);
    arduboy.print(F("B:Highscores"));

    arduboy.display();
}


// Highscore Menu
void updateHighScores()
{
    if(arduboy.justPressed(B_BUTTON))
        gameState = GameState::End;
}

void drawHighscores()
{
    arduboy.setCursor( textToMiddle(10), 5);
    arduboy.print(F("Highscores:"));

    arduboy.setCursorY(15);
    if(saveData.highscores[0].score < 100)
        arduboy.setCursorX(textToMiddle(8));
    else if(saveData.highscores[0].score < 1000)
        arduboy.setCursorX(textToMiddle(9));
    else if(saveData.highscores[0].score < 10000)
        arduboy.setCursorX(textToMiddle(10));
    else
        arduboy.setCursorX(textToMiddle(11));

    arduboy.print(F("1:"));
    arduboy.print(saveData.highscores[0].score);
    arduboy.print(F(" "));
    arduboy.print(saveData.highscores[0].name[0]);
    arduboy.print(saveData.highscores[0].name[1]);
    arduboy.print(saveData.highscores[0].name[2]);

    arduboy.setCursorY(25);
    if(saveData.highscores[1].score < 100)
        arduboy.setCursorX(textToMiddle(8));
    else if(saveData.highscores[1].score < 1000)
        arduboy.setCursorX(textToMiddle(9));
    else if(saveData.highscores[1].score < 10000)
        arduboy.setCursorX(textToMiddle(10));
    else
        arduboy.setCursorX(textToMiddle(11));

    arduboy.print(F("2:"));
    arduboy.print(saveData.highscores[1].score);
    arduboy.print(F(" "));
    arduboy.print(saveData.highscores[1].name[0]);
    arduboy.print(saveData.highscores[1].name[1]);
    arduboy.print(saveData.highscores[1].name[2]);

    arduboy.setCursorY(35);
    if(saveData.highscores[2].score < 100)
        arduboy.setCursorX(textToMiddle(8));
    else if(saveData.highscores[2].score < 1000)
        arduboy.setCursorX(textToMiddle(9));
    else if(saveData.highscores[2].score < 10000)
        arduboy.setCursorX(textToMiddle(10));
    else
        arduboy.setCursorX(textToMiddle(11));

    arduboy.print(F("3:"));
    arduboy.print(saveData.highscores[2].score);
    arduboy.print(F(" "));
    arduboy.print(saveData.highscores[2].name[0]);
    arduboy.print(saveData.highscores[2].name[1]);
    arduboy.print(saveData.highscores[2].name[2]);

    arduboy.setCursor( textToMiddle(6), 45);
    arduboy.print(F("B:Back"));
}

char alphabet[27] =
{
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z',
};

void updateName()
{
    updateNameCursor();

    if(arduboy.justPressed(DOWN_BUTTON) && letter[nameCursor] < 26)
        letter[nameCursor] += 1;
    if(arduboy.justPressed(UP_BUTTON) && letter[nameCursor] > 0)
        letter[nameCursor] -= 1;

    if(arduboy.justPressed(A_BUTTON))
    {
        if(place == 1)
        {
            for(size_t i = 0; i < 3; ++i)
                saveData.highscores[2].name[i] = saveData.highscores[1].name[i];
            for(size_t i = 0; i < 3; ++i)
                saveData.highscores[1].name[i] = saveData.highscores[0].name[i];
            for(size_t i = 0; i < 3; ++i)
                saveData.highscores[0].name[i] = alphabet[letter[i]];
        }
        else if(place == 2)
        {
            for(size_t i = 0; i < 3; ++i)
                saveData.highscores[2].name[i] = saveData.highscores[1].name[i];
            for(size_t i = 0; i < 3; ++i)
                saveData.highscores[1].name[i] = alphabet[letter[i]];
        }
        else
        {
            for(size_t i = 0; i < 3; ++i)
                saveData.highscores[2].name[i] = alphabet[letter[i]];
        }

        saveSaveData();
        nameEntered = true;
        gameState = GameState::HighScore;
    }

}

const unsigned char DownArrow[] PROGMEM = 
{
    //Dimensions
    6, 9,

    // Frame 0 - DownArrow
    0x20, 0x40, 0x80, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void drawName()
{
    arduboy.setCursor( textToMiddle(14), 2 );
    arduboy.print(F("New Highscore!"));

    arduboy.setCursor( (textToMiddle(3) + (nameCursor * 6)), 14 );
    arduboy.print(F("^"));

    Sprites::drawSelfMasked( (textToMiddle(3) + (nameCursor * 6)), 24, DownArrow, 0 );

    arduboy.setCursor( textToMiddle(3), 20 );
    arduboy.print(F("___"));

    arduboy.setCursor( textToMiddle(3), 18 );
    arduboy.print(alphabet[letter[0]]);
    arduboy.print(alphabet[letter[1]]);
    arduboy.print(alphabet[letter[2]]);
}

void updateNameCursor()
{
    if(arduboy.justPressed(RIGHT_BUTTON) && nameCursor < 2)
        nameCursor += 1;
    else if(arduboy.justPressed(LEFT_BUTTON) && nameCursor > 0)
        nameCursor -= 1;
}