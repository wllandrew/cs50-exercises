#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int evaluate(string word);

int main(void)
{
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");
    if (evaluate(word1) > evaluate(word2))
    {
        printf("Player 1 wins!\n");
    }
    else if (evaluate(word1) == evaluate(word2))
    {
        printf("Tie!\n");
    }
    else
    {
        printf("Player 2 wins!\n");
    }
}

int evaluate(string word)
{
    int sum = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        char cr = tolower(word[i]);

        if (cr == 'a' || cr == 'e' || cr == 'i' || cr == 'l' || cr == 'n' || cr == 'o' || cr == 'r' || cr == 't' || cr == 's' || cr == 'u')
        {
            sum += 1;
        }
        else if (cr == 'b' || cr == 'c' || cr == 'm' || cr == 'p')
        {
            sum += 3;
        }
        else if (cr == 'd' || cr == 'g')
        {
            sum += 2;
        }
        else if (cr == 'f' || cr == 'h' || cr == 'v' || cr == 'w' || cr == 'y')
        {
            sum += 4;
        }
        else if (cr == 'k')
        {
            sum += 5;
        }
        else if (cr == 'j' || cr == 'x')
        {
            sum += 8;
        }
        else if (cr == 'q' || cr == 'z')
        {
            sum += 10;
        }
    }
    return sum;
}
