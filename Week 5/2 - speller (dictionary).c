#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

const unsigned int N = 26;

unsigned int word_count = 0;

node *table[N];

bool check(const char *word)
{
    int index = hash(word);

    for (node* n = table[index]; n != NULL; n = n->next)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
    }

    return false;
}

unsigned int hash(const char *word)
{
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE* dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }

    // Char takes 1 byte
    char word[LENGTH + 1];

    while (fscanf(dic, "%s", word) != EOF)
    {
        node* new = malloc(sizeof(node));
        if (new == NULL)
        {
            return false;
        }
        strcpy(new->word, word);
        new->next = NULL;

        int index = hash(word);

        if (table[index] == NULL)
        {
            table[index] = new;
        }
        else
        {
            new->next = table[index];
            table[index] = new;
        }

        word_count++;
    }
    fclose(dic);

    return true;
}

unsigned int size(void)
{
    // Ao contar no próprio loading, a função size é minimizada.
    return word_count;
}

// Porque essa função retorna um bool?
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node* n = table[i];

        while(n != NULL)
        {
            node* old = n;
            n = n->next;
            free(old);
        }
    }

    return true;
}
