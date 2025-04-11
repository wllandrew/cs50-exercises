#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

#define LQTD 26
#define ASCIIUPPER 65

string cipher(string text, string cipher);
int validation(string cipher);
string to_upper_string(string cipher);

int main(int argc, string argv[])
{
    if (argc > 2 || argc < 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        string cipher1 = to_upper_string(argv[1]);

        if (validation(cipher1) == 1)
        {
            return 1;
        }

        string text = get_string("plaintext: ");
        printf("ciphertext: %s\n", cipher(text, cipher1));

        return 0;
    }
}

string cipher(string text, string cipher)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char current = text[i];

        if (isalpha(current)) {
            if (islower(current)) {
                current = toupper(current);
                int index = ((int) current - ASCIIUPPER) % LQTD;
                text[i] = tolower(cipher[index]);
            }
            else
            {
                int index = ((int) current - ASCIIUPPER) % LQTD;
                text[i] = cipher[index];
            }

        }
    }

    return text;
}

int validation(string cipher)
{
    for (int i = 0; i < LQTD; i++)
    {
        char cr = cipher[i];

        if (!isalpha(cr))
        {
            return 1;
        }

        for (int j = 0; j < LQTD; j++)
        {
            if (j != i && cipher[j] == cr)
            {
                return 1;
            }
        }
    }
    return 0;
}

string to_upper_string(string text)
{
    for (int i = 0; i < LQTD; i++)
    {
        text[i] = toupper(text[i]);
    }
    return text;
}
