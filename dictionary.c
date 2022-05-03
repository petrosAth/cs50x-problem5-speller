// Implements a dictionary's functionality
#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "dictionary.h"

// Prototypes
void loadWord(char *word, int hash);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1150;

// Number of lines loaded in dictionary
uint32_t loadedLines = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Lowercase string word
    char tmpWord[strlen(word)];
    for (int i = 0; i <= strlen(word); i++)
    {
        tmpWord[i] = tolower(word[i]);
    }

    // Check if string word is in loaded dictionary
    for (node *checkTmp = table[hash(tmpWord) + 1]; checkTmp != NULL; checkTmp = checkTmp->next)
    {
        if (strncmp(checkTmp->word, tmpWord, strlen(word)) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash calculation based on first letter ascii code and word length
    return (((tolower(word[0]) - 97) * 45) + strlen(word));
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dictionaryFile = fopen(dictionary, "r");
    if (dictionaryFile == NULL)
    {
        return 1;
    }

    // Read dictionary lines
    char *lineBuffer = NULL;
    uint64_t lineBufferSize = 0;
    int lineSize = 0;
    char endOfFile = ' ';

    // Initialize table[N]
    for (int i = 0; i < N; ++i)
    {
        table[i] = NULL;
    }

    // Read dictionary loop
    while (lineSize >= 0)
    {
        lineSize = getline(&lineBuffer, &lineBufferSize, dictionaryFile);
        if (lineSize >= 0)
        {
            loadWord(lineBuffer, hash(lineBuffer));
            ++loadedLines;
        }
    }

    // Close dictionary file
    free(lineBuffer);
    fclose(dictionaryFile);

    // Check if getline reached eof without errors
    if ((lineSize == -1) && (loadedLines != 0))
    {
        return true;
    }

    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (loadedLines > 0)
    {
        return loadedLines;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Free allocated space from linked list
    uint32_t unloadedLines = 0;
    for (int i = 0; i < N; ++i)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
            ++unloadedLines;
        }
    }

    if (unloadedLines == loadedLines)
    {
        return true;
    }

    return false;
}

// Load word in dictionary
void loadWord(char *word, int hash)
{
    node *newWord = malloc(sizeof(node));
    if (newWord == NULL)
    {
        exit(1);
    }
    strcpy(newWord->word, word);
    newWord->next = table[hash];
    table[hash] = newWord;
}