#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int get_score(string text);

int main(void)
{
    string text = get_string("Text: ");
    int score = get_score(text);

    if (score < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (score > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", score);
    }
}

int get_score(string text)
{
    int word_count = 0, char_count = 0, sentence_count = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char current = text[i];

        if (isalpha(current))
        {
            char_count++;
        }
        if (current == ' ')
        {
            word_count++;
        }
        if (current == '!' || current == '?' || current == '.')
        {
            sentence_count++;
        }
    }
    word_count++;

    float L = ((float) char_count / (float) word_count) * 100;
    float S = ((float) sentence_count / (float) word_count) * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    return round(index);
}
