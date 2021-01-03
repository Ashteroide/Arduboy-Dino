#pragma once

// NameEntryState
struct NameEntry
{
    uint8_t nameCursor;
    uint8_t letter[3];

    void update()
    {
        if(arduboy.justPressed(A_BUTTON))
        {
            shift();
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

        arduboy.setCursor( textToMiddle(6 + countDigits(gamePlay.score)), 35);
        arduboy.print(F("Score:"));
        arduboy.print(gamePlay.score);

        arduboy.setCursor( textToMiddle(12), 45 );
        arduboy.print(F("A:Enter Name"));
    }

    void shift()
    {
        for(size_t index = 0; index < 3; ++index)
        {
            if(gamePlay.score > data.highscores[index].score)
            {
                for(size_t nextIndex = 2; nextIndex > index; --nextIndex)
                    data.highscores[nextIndex] = data.highscores[nextIndex - 1];

                data.highscores[index].score = gamePlay.score;

                for(size_t letterIndex = 0; letterIndex < 3; ++letterIndex)
                    data.highscores[index].name[letterIndex] = alphabet[letter[letterIndex]];
                
                break;
            }
        }
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
NameEntry nameEntry;