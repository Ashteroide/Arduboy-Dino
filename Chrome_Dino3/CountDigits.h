#pragma once

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