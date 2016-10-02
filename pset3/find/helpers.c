/**
 * helpers.c
 * 
 * Andriy Doroshenko
 * mpleukraine@gmail.com
 * 
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n == 1)
    {
        return values[0] == value;
    }
    int mid_pos = n / 2;
    if (value <= values[mid_pos - 1])
    {
        return search(value, values, mid_pos);
    }
    else 
    {
        return search(value, values + mid_pos, n - mid_pos);
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // implement an O(n^2) sorting algorithm
    // insertion sort
    for (int i = 1; i < n; i++)
    {
        int candidate = values[i];
        int ins_place = i;
        do {
            --ins_place;
            if (candidate < values[ins_place]) 
            {
                values[ins_place + 1] = values[ins_place];
                values[ins_place] = candidate;
            }
        } while (ins_place > 0 && values[ins_place - 1] > candidate);
    }
    
    return;
}