// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 2048

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//create a new node (This block used the tutorial available at http://y2u.be/VOpjAHCee7c)
node *createNewNode(char *word)
{
    node *result = malloc(sizeof(struct node));
    strcpy(result->word, word);
    result->next = NULL;
    return result;
}

//initialize the variables
int num_words = 0;
node *hashtable[N];

/* This makes use of the polynomial rolling hash function: https://en.wikipedia.org/wiki/Rolling_hash#Polynomial_rolling_hash
    In particular, I am using the first two letters of a word (as suggested by Dr. Malan in lecture). h = word[0] + p*word[1].
    In this function, a = 1, b = 2, c = 3... etc (otherwise a == aa == aaa = 0 which is bad)
    P should be a prime number.  Since we used only lowercase, in this case P = 31;

    Rationale: In theory this can cut the "wall-clock" time down from O(n/26) to something smaller.
 */
unsigned int hash(const char *word)
{
    if (strlen(word) == 1)
    {
        /*the only two one-letter words in English are 'a' and 'I', so it is easy to create special cases for that */
        return (1);
    }
    else
    {
    int first = tolower(word[0]) - 'a' + 1;
    int second = tolower(word[1]) - 'a' + 1;
    int hash_value = abs(first + 31 * second); //take the absolute value to handle the case containing (apostrophe)
    return hash_value;
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH];

    while (fscanf(file, "%s", word) != EOF)
    {

        int h = hash(word);
        node *temp = createNewNode(word); //create a new node contaiing the word
        //if the hash already exists doesn't yet exist, create the first term in the hash table
        if (temp != NULL)
        {
            temp->next = hashtable[h];
        }

        hashtable[h] = temp;
        num_words++;

    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return num_words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int n = strlen(word);
    char cword[n + 1];

    for (int i = 0; i < n; i++)
    {
        cword[i] = tolower(word[i]);
    }
    cword[n] = '\0';


    int k = hash(cword);
    node *q = hashtable[k];
    while (q != NULL)
    {
        if (strcmp(q->word, cword) != 0)
        {
            q = q->next;
        }
        else
        {
            return true;
        }
    }
    return(false);
}

// Unloads dictionary from memory, returning true if successful else false
// Basically following the instructions in the walkthrough, but you need to iterate through the original array/
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
        return true;
}