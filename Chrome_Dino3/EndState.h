#pragma once

// EndState
struct EndState
{
    // SetCursorForScore
    void setCursorForScore(uint8_t x, uint8_t y)
    {
        arduboy.setCursorY(y);

        if(gamePlay.score < 100)
            arduboy.setCursorX(textToMiddle(x + 0));
        else if(gamePlay.score < 1000)
            arduboy.setCursorX(textToMiddle(x + 1));
        else if(gamePlay.score < 10000)
            arduboy.setCursorX(textToMiddle(x + 2));
        else
            arduboy.setCursorX(textToMiddle(x + 3));
    }

    void update()
    {
        if(gamePlay.score > data.highscores[2].score && !dino.autoJump)
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
        arduboy.print(gamePlay.score);

        arduboy.setCursor( textToMiddle(9), 42 );
        arduboy.print(F("A:Restart"));

        arduboy.setCursor( textToMiddle(12), 52 );
        arduboy.print(F("B:Highscores"));

        arduboy.display();
    }
};
EndState endState;
