// Chrome Dino v3
// Created: 22 October, 2020
// Ashlee J (Ashteroide)

#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "Images.h"

// Menu Cusor
enum class MenuCursor
{
    Start,
    AI,
    Sound
};
MenuCursor menuCursor = MenuCursor::Start;

// Game State
enum class GameState
{
    Menu,
    Game,
    End,
    HighScore,
    NameEntry
};
GameState gameState = GameState::Menu;

#include "Utilities.h"

#include "Data.h"

// DinoState
enum class DinoState
{
    updateJumpingState,
    updateFallingState,
    updateRunningState,
    updateDuckingState,
};
DinoState dinoState = DinoState::updateRunningState;

#include "GamePlayState.h"
#include "MenuState.h"
#include "EndState.h"

#include "Alphabet.h"
#include "HighScoreState.h"
#include "NameEntryState.h"

void setup()
{
    arduboy.begin();
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
            menuState.update();
            menuState.draw();
            break;

        case GameState::Game:
            gamePlay.update();
            gamePlay.draw();
            break;

        case GameState::End:
            endState.update();
            endState.draw();
            break;
        
        case GameState::HighScore:
            highScoreState.update();
            highScoreState.draw();
            break;

        case GameState::NameEntry:
            nameEntry.update();
            nameEntry.draw();
            break;
    }

    arduboy.display();
}