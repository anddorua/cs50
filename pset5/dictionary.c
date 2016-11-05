/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"
#include "fnv.h"
#define RESERVE 3

typedef struct MapNode_t {
    char *value;
    struct MapNode_t *next;
} MapNode_t;

typedef struct MapNode_t* MapNodePtr;

MapNodePtr* hash_map;
int dictionary_count = 0;

void addToHashMap(uint16_t index, char* value)
{
    MapNodePtr* tail = &(hash_map[index]);
    while(*tail) {
        MapNodePtr nodePtr = *tail;
        tail = &(nodePtr->next);
    }
    MapNodePtr new_node = calloc(1, sizeof(MapNode_t));
    int s_len = strlen(value);
    new_node->value = malloc(s_len + 1);
    strcpy(new_node->value, value);
    *tail = new_node;
    dictionary_count++;
}

uint16_t hash(char* str)
{
    Fnv32_t hash_val = fnv_32a_str(str, FNV1_32_INIT);
    return hash_val;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    char buf[LENGTH + RESERVE];
    int i;
    for(i = 0; word[i]; i++){
        buf[i] = tolower(word[i]);
    }
    buf[i] = 0;
    
    uint16_t hash_val = hash(buf);
    MapNodePtr node;
    if (!(node = hash_map[hash_val])) {
        return false;
    }
    int cmp;
    do {
        if ((cmp = strcmp(node->value, buf)) == 0) {
            return true;
        };
        if (cmp > 0) {
            return false;
        }
        node = node->next;
    } while(node);
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    hash_map = calloc(UINT16_MAX + 1, sizeof(MapNodePtr));
    if (!hash_map) {
        fprintf(stderr, "Could not allocate memory for hash map of size %lu\n", sizeof(MapNodePtr) * (UINT16_MAX + 1));
        return false;
    }
    
    dictionary_count = 0;

    FILE* f = fopen(dictionary, "r");
    if (!f) {
        fprintf(stderr, "Could not open dictionary file %s\n", dictionary);
        return false;
    }
    char buf[LENGTH + RESERVE];
    char *read_result;
    while (!feof(f))
    {
        read_result = fgets(buf, LENGTH + RESERVE, f);
        if (!read_result || !isalpha(buf[0])) {
            continue;
        }
        // stripe trailing new line character
        if ((read_result = strchr(buf, '\n')) != NULL) {
            *read_result = '\0';
        }
        uint16_t hash_val = hash(buf);
        addToHashMap(hash_val, buf);
    }
    fclose(f);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dictionary_count;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    if (!hash_map) {
        return true;
    }
    for (int i = 0; i <= UINT16_MAX; i++ ) {
        // free nodes chain
        MapNodePtr tail = hash_map[i];
        while(tail) {
            MapNodePtr next_node = tail->next;
            free(tail->value);
            free(tail);
            tail = next_node;
        }

    }
    free (hash_map);
    return true;
}
