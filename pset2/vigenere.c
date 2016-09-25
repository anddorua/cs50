#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

char encrypt_char(char to_encrypt, char key);
bool has_non_alpha(string key);

int main(int argc, string argv[])
{
    if (argc != 2 || has_non_alpha(argv[1])) {
        printf("Usage: vigenere <pass>\n");
        printf("\twhere <pass> - encryption password\n");
        return 1;
    }
    string key = argv[1];
    
    string msg = GetString();
    
    for (int n = strlen(msg), i = 0, k = 0, key_len = strlen(key); i < n; i++)
    {
        char out = msg[i];
        if (isalpha(out)) {
            out = encrypt_char(out, key[k++]);
            k = k == key_len ? 0 : k;
        }
        printf("%c", out);
    }
    printf("\n");
    
    return 0;
}

char encrypt_char(char to_encrypt, char key)
{
    key = islower(key) ? key - 'a' : key - 'A';
    char base = islower(to_encrypt) ? 'a' : 'A';
    to_encrypt = ((to_encrypt - base + key) % 26) + base;
    return to_encrypt;
}

bool has_non_alpha(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isalpha(key[i]))
            return true;
    }
    return false;
}