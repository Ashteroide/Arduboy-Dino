#pragma once

// HighScoreState
struct HighScoreState
{
    void update()
    {
        if(arduboy.justPressed(B_BUTTON))
            gameState = ChromeDino::Menu;
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