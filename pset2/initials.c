#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

int main(void)
{
    string name = GetString();
    
    for (int n = strlen(name), i = 0, newPart = true; i < n; i++)
    {
        if (isalpha(name[i]))
        {
            if (newPart)
            {
                printf("%c", toupper(name[i]));
                newPart = false;
            }
        } 
        else 
        {
            newPart = true;
        }
    }
    printf("\n");
}