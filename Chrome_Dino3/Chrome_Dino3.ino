// Chrome Dino v3
// Created: 22 October, 2020
// Ashlee J (Ashteroide)

#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "Images.h"

// Screen Structure
struct Dimensions
{
    static constexpr uint8_t width = Arduboy2::width();
    static constexpr uint8_t height = Arduboy2::height();
};
Dimensions;

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

#include "TextToMiddle.h"

#include "Data.h"

// DinoState
enum class DinoState
{
    Jumping,
    Falling,
    Running,
    Ducking,
};
DinoState dinoState = DinoState::Running;

#include "Ptero.h"
#include "Cactus.h"
#include "Cloud.h"
#include "Dino.h"

#include "GamePlay.h"

#include "MenuState.h"
#include "EndState.h"

#include "Alphabet.h"

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

#include "HighScores.h"
#include "NameEntry.h"

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
            gamePlay.update();
            gamePlay.draw();
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
            nameEntry.updateNameCursor();
            nameEntry.update();
            nameEntry.draw();
            break;
    }

    arduboy.display();
}