// Chrome Dino v3
// Created: 22 October, 2020
// Ashlee J (Ashteroide)

#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "Images.h"

#include "MenuCursor.h"
MenuCursor menuCursor = MenuCursor::Start;

#include "GameState.h"
GameState gameState = GameState::Menu;

#include "Utilities.h"

#include "Data.h"

#include "DinoState.h"
DinoState dinoState = DinoState::updateRunningState;

#include "GamePlayState.h"
GamePlayState gamePlay;

#include "MenuState.h"
MenuState menuState;

#include "EndState.h"
EndState endState;

#include "Alphabet.h"
#include "HighScoreState.h"
HighScoreState highScoreState;

#include "NameEntryState.h"
NameEntryState nameEntryState;

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
            nameEntryState.update();
            nameEntryState.draw();
            break;
    }

    arduboy.display();
}