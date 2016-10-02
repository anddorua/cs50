#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <cs50.h>
#include "../helpers.h"

int main(int argc, string argv[])
{
    /*
    if (argc != 2) {
        printf("usage: sort_test");
        return 1;
    }
    */
    
    const int MAX_BUFFER = 1024;
    
    int amount;
    int to_sort[MAX_BUFFER];
    for (amount = 0; amount < MAX_BUFFER; amount++)
    {
        int value = GetInt();
        if (value == INT_MAX) 
        {
            break;
        }
        else 
        {
            to_sort[amount] = value;
        }
    }

    sort(to_sort, amount);
    
    bool error_found = false;
    for (int i = 0; i < amount; i++)
    {
        printf("a[%d] = %d", i, to_sort[i]);
        if (i == 0 || to_sort[i] >= to_sort[i - 1]) 
        {
            printf(" - ok\n");
        }
        else 
        {
            printf(" - ERROR!\n");
            error_found = true;
        }
    }
    if (error_found)
    {
        printf("there was errors!\n");
    } 
    else
    {
        printf("everithing ok.\n");
    }
}