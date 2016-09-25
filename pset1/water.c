#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int minutes;
    printf("minutes: ");
    do {
        minutes = GetInt();
        if (minutes <= 0) {
            printf("enter positive number of minutes: ");
        }
    } while (minutes <= 0);
    printf("bottles: %d\n", minutes * 192 / 16);
}