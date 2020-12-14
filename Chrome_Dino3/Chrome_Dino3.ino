// Chrome Dino v3
// Created: 22 October, 2020
// Ashlee J (Ashteroide)

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

// Object Structure
struct Object
{
    float x, y, spd, accel;
};

// Menu Cusor
enum class MenuCursor
{
    Start,
    AI,
    Sound
};
MenuCursor menuCursor = MenuCursor::Start;

// Game State
enum class ChromeDino
{
    Menu,
    Game,
    End,
    HighScore,
    NameEntry
};
ChromeDino gameState = ChromeDino::Menu;

int textToMiddle(int charWidth)
{
    return (Dimensions::width - (arduboy.getCharacterWidth(charWidth) + arduboy.getCharacterSpacing(charWidth - 1))) / 2;
}

// Data
struct Data
{
    struct HighScoreEntry
    {
        uint16_t score;
        char name[3];
    };
    HighScoreEntry highscores[3];

    static constexpr uint16_t saveDataLocation = 272;

    void save()
    {
        EEPROM.put(saveDataLocation, highscores);
    }

    void load()
    {
        EEPROM.get(saveDataLocation, highscores);
    }

    void clear()
    {
        auto data2 = reinterpret_cast<uint8_t *>(&highscores);
        for(size_t index = 0; index < sizeof(highscores); ++index)
            data2[index] = 0;
        
        save();
    }
};
Data data;

// DinoState
enum class DinoState
{
    Jumping,
    Falling,
    Running,
    Ducking,
};
DinoState dinoState = DinoState::Running;

// Ptero
Object ptero { Dimensions::width, (Dimensions::height - dinoHeight - random(6, 10)), 2, 0.02 };

// PteroState
struct PteroState
{
    bool Spawn;

    void update()
    {
        /*
        if(ptero.x > -pteroWidth && pteroState.Spawn)
            ptero.x -= ptero.spd;
        else if(pteroState.Spawn)
        {
            ptero.x = Dimensions::width + random(pteroWidth, 100);
            pteroState.Spawn = !pteroState.Spawn;
        }
        */
    }

    void draw()
    {
        /*
        if( (game.frame % (scoreInterval / 2)) != 0)
            Sprites::drawSelfMasked(ptero.x, ptero.y, pteroImg, 0);
        else
            Sprites::drawSelfMasked(ptero.x, ptero.y, pteroImg, 2);

        */
    }
};
PteroState pteroState;

// Cactus
Object cactus { Dimensions::width, 43, 2, 0.02 };

// CactusState
struct CactusState
{
    void update()
    {
        if(cactus.x > -20)
            cactus.x -= cactus.spd;
        else
        {
            cactus.x = Dimensions::width + random(cactusWidth, Dimensions::width);
            cactus.spd += cactus.accel;
            pteroState.Spawn = true;
        }

    }

    void draw()
    {
        Sprites::drawSelfMasked(cactus.x, cactus.y, cactusImg, 0);
    }
};
CactusState cactusState;

Object cloud { Dimensions::width + cloudWidth, 10, 1, 0 };

// Cloud
struct CloudState
{
    void update()
    {
        if(cloud.x < -cloudWidth)
            cloud.x = Dimensions::width + random(cloudWidth, 100);
        else
            cloud.x -= cloud.spd;
    }

    void draw()
    {
        Sprites::drawSelfMasked(cloud.x, cloud.y, cloudImg, 0);
    }
};
CloudState cloudState;

// Dino
struct Dino
{
    static constexpr uint8_t step = 10;
    static constexpr uint8_t maxJump = 8;
    static constexpr uint8_t x = 5;
    uint16_t y = (Dimensions::height - 2 - dinoHeight);
    int jumpVel = 0;
    bool autoJump = false;

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

            if(game.soundMode)
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

            if(game.soundMode)
                sound.tone(500, 50);
        }

        else if(arduboy.pressed(DOWN_BUTTON))
        {
            dinoState = DinoState::Ducking;

            if(game.soundMode)
                sound.tone(250, 50);
        }

        if( ((game.frame % step) / 4 != 0))
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

        if((game.frame % step) / 4 != 0)
            Sprites::drawSelfMasked(x, y, dinoDuckImg, 0);
        else
            Sprites::drawSelfMasked(x, y, dinoDuckImg, 1);
    }
};
Dino dino;

struct StartState
{
    void update()
    {
        if(arduboy.justPressed(DOWN_BUTTON) && menuCursor != MenuCursor::AI)
            menuCursor = MenuCursor::AI;

        arduboy.setCursor( (textToMiddle(8) / 2), 30);
        arduboy.print(F("A>"));
    }
};
StartState startState;

struct AutoJumpState
{
    void update()
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
};
AutoJumpState autoJumpState;

struct SoundState
{
    void update()
    {
        if(arduboy.justPressed(A_BUTTON))
            game.soundMode = !game.soundMode;

        if(arduboy.justPressed(UP_BUTTON))
            menuCursor = MenuCursor::AI;

        arduboy.setCursor( (textToMiddle(4) / 2), 46);
        arduboy.print(F("A>"));
    }
};
SoundState soundState;

// SetCursorForScore
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

// PlayState
struct PlayState
{

    void update()
    {
        dino.update();
        pteroState.update();
        cactusState.update();
        cloudState.update();

        ++game.frame;

        if((game.frame % scoreInterval) != 0)
            ++game.score;
    }

    void draw()
    {
        arduboy.drawLine(0, groundHeight, Dimensions::width, groundHeight);

        setCursorForScore(2, 5);

        arduboy.print(game.score);

        pteroState.draw();
        cactusState.draw();
        cloudState.draw();
    }
};
PlayState playState;

struct MenuState
{
    void reset()
    {
        cactus = { Dimensions::width, 43, 2, 0.02 };
        cloud = { Dimensions::width + cloudWidth, 10, 1, 0 };

        arduboy.initRandomSeed();

        dinoState = DinoState::Running;

        game = { 0, 0, game.soundMode };

        data.load();
    }

    void update()
    {
        if(arduboy.justPressed(A_BUTTON) && menuCursor == MenuCursor::Start)
        {
            gameState = ChromeDino::Game;
            reset();
        }

        switch(menuCursor)
        {
            case MenuCursor::Start:
                startState.update();
                break;

            case MenuCursor::AI:
                autoJumpState.update();
                break;

            case MenuCursor::Sound:
                soundState.update();
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
};
MenuState menuState;

// EndState
struct EndState
{
    void update()
    {
        if(game.score > data.highscores[2].score && !dino.autoJump)
            gameState = ChromeDino::NameEntry;
        

        if(arduboy.justPressed(B_BUTTON))
            gameState = ChromeDino::HighScore;
        else if(arduboy.justPressed(A_BUTTON))
            gameState = ChromeDino::Menu;
    }

    void draw()
    {
        arduboy.drawLine(0, 20, Dimensions::width, 20);
        Sprites::drawSelfMasked( (Dimensions::width - dinoDuckWidth) / 2, 12, dinoDuckImg, 2 );
        Sprites::drawSelfMasked( 20, (21 - cactusHeight), cactusImg, 0 );

        arduboy.setCursor( textToMiddle(10), 22 );
        arduboy.print(F("GAME OVER!"));

        setCursorForScore(8, 32);

        arduboy.print(F("Score:"));
        arduboy.print(game.score);

        arduboy.setCursor( textToMiddle(9), 42 );
        arduboy.print(F("A:Restart"));

        arduboy.setCursor( textToMiddle(12), 52 );
        arduboy.print(F("B:Highscores"));

        arduboy.display();
    }
};
EndState endState;

char alphabet[27] =
{
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z'
};

const unsigned char DownArrow[] PROGMEM = 
{
    //Dimensions
    6, 9,

    // Frame 0 - DownArrow
    0x20, 0x40, 0x80, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Name Variables
uint8_t letter[3], nameCursor;

// HighScoreState
struct HighScoreState
{
    void update()
    {
        if(arduboy.justPressed(B_BUTTON))
            gameState = ChromeDino::Menu;
    }

    void shift()
    {
        for(size_t index = 0; index < 3; ++index)
        {
            if(game.score > data.highscores[index].score)
            {
                for(size_t nextIndex = 2; nextIndex > index; --nextIndex)
                    data.highscores[nextIndex] = data.highscores[nextIndex - 1];

                data.highscores[index].score = game.score;

                for(size_t letterIndex = 0; letterIndex < 3; ++letterIndex)
                    data.highscores[index].name[letterIndex] = alphabet[letter[letterIndex]];
                
                break;
            }
        }
    }

    void draw()
    {
        arduboy.setCursor( textToMiddle(10), 5);
        arduboy.print(F("Highscores:"));

        for(size_t index = 0; index < 3; ++index)
        {
            arduboy.setCursorY( (15 + (index * 10)) );

            arduboy.setCursorX( textToMiddle(countDigits(data.highscores[index].score) + 5) );

            arduboy.print(index + 1);
            arduboy.print(F(":"));
            arduboy.print(data.highscores[index].score);
            arduboy.print(F(" "));
            for(size_t letterIndex = 0; letterIndex < 3; ++letterIndex)
                arduboy.print(data.highscores[index].name[letterIndex]);
        }

        arduboy.setCursor( textToMiddle(9), 45);
        arduboy.print(F("B:Restart"));
    }
};
HighScoreState highScoreState;

// CountDigits in Number
int countDigits(uint16_t number)
{
    int count = 0;

    while(number != 0)
    {
        number = number / 10;
        ++count;
    }
    return count;
}

// NamEntryState
struct NameEntryState
{
    void update()
    {
        if(arduboy.justPressed(A_BUTTON))
        {
            highScoreState.shift();
            data.save();

            nameCursor = 0;

            gameState = ChromeDino::HighScore;
        }
    }

    void draw()
    {
        arduboy.setCursor( textToMiddle(14), 2 );
        arduboy.print(F("New Highscore!"));

        arduboy.setCursor( (textToMiddle(3) + (nameCursor * 6)), 14 );
        arduboy.print(F("^"));

        Sprites::drawSelfMasked( (textToMiddle(3) + (nameCursor * 6)), 24, DownArrow, 0 );

        arduboy.setCursor( textToMiddle(3), 20 );
        arduboy.print(F("___"));

        arduboy.setCursor( textToMiddle(3), 18 );
        for (size_t index = 0; index < 3; index++)
            arduboy.print(alphabet[letter[index]]);

        arduboy.setCursor( textToMiddle(6 + countDigits(game.score)), 35);
        arduboy.print(F("Score:"));
        arduboy.print(game.score);

        arduboy.setCursor( textToMiddle(12), 45 );
        arduboy.print(F("A:Enter Name"));
    }

    void updateNameCursor()
    {
        if(arduboy.justPressed(RIGHT_BUTTON) && nameCursor < 2)
            nameCursor += 1;
        else if(arduboy.justPressed(LEFT_BUTTON) && nameCursor > 0)
            nameCursor -= 1;

        if(arduboy.justPressed(DOWN_BUTTON) && letter[nameCursor] < 26)
            letter[nameCursor] += 1;
        else if(arduboy.justPressed(UP_BUTTON) && letter[nameCursor] > 0)
            letter[nameCursor] -= 1;
    }
};
NameEntryState nameEntryState;

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
        case ChromeDino::Menu:
            menuState.update();
            menuState.draw();
            break;

        case ChromeDino::Game:
            playState.update();
            playState.draw();
            break;

        case ChromeDino::End:
            endState.update();
            endState.draw();
            break;
        
        case ChromeDino::HighScore:
            highScoreState.update();
            highScoreState.draw();
            break;

        case ChromeDino::NameEntry:
            nameEntryState.updateNameCursor();
            nameEntryState.update();
            nameEntryState.draw();
            break;
    }

    arduboy.display();
}