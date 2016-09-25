#include <stdio.h>
#include <cs50.h>
#include <math.h>

/**
 * Returns maxium of coins of specified rating (i.e. c25, c10 e.t.c.)
 * could be given from money_in_cents. Also returns change.
 **/
int GetCoins(int money_in_cents, int rating, int *change)
{
    *change = money_in_cents % rating;
    return money_in_cents / rating;
}

int main(void)
{
    float money_in_dollars;
    do {
        printf("O hai! How much change is owed?\n");
        money_in_dollars = GetFloat();
    } while (money_in_dollars < 0.0);

    int money_in_cents = round(money_in_dollars * 100.0);
    int rating[] = {25, 10, 5, 1, 0};
    int total_coins = 0;
    int i = 0;
    while (money_in_cents > 0 && rating[i] != 0) {
        total_coins += GetCoins(money_in_cents, rating[i++], &money_in_cents);
    }
    
    printf("%d\n", total_coins);
}