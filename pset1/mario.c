#include <stdio.h>
#include <cs50.h>

void PrintSeries(string to_print, int times)
{
    for (int i = 0; i < times; i++) {
        printf("%s", to_print);
    }
}

int main(void)
{
    int height;
    do {
        printf("Height: ");
        height = GetInt();
    } while (height < 0 || height > 23);
    
    for (int i = 1; i <= height; i++) {
        PrintSeries(" ", height - i);
        PrintSeries("#", i + 1);
        printf("\n");
    }
}