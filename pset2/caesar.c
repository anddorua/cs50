#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

int main(int argc, string argv[])
{
    if (argc != 2) {
        printf("Usage: caesar <n>\n");
        printf("\twhere <n> - non negative int number\n");
        return 1;
    }
    int k = atoi(argv[1]);
    
    string msg = GetString();
    
    for (int n = strlen(msg), i = 0; i < n; i++)
    {
        char out = msg[i];
        if (isalpha(out)) {
            char base = islower(out) ? 'a' : 'A';
            out = ((out - base + k) % 26) + base;
        }
        printf("%c", out);
    }
    printf("\n");
    
    return 0;
}
