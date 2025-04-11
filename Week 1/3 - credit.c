#include <stdio.h>
#include <cs50.h>

#define BASE 10

long get_credit_card();
int checksum(long cc);
int final_check(long cc);

int main(void)
{
    long credit_card = get_credit_card();
    int check = checksum(credit_card);
    if (check == 1)
    {
        int final = final_check(credit_card);
    }
    else
    {
        printf("INVALID\n");
    }
}

// Returns the credit card number

long get_credit_card()
{
    long cc;
    do
    {
        cc = get_long("Number: ");
    } while (cc < 0);
    return cc;
}

// Just the even numbers get selected, multiplied and we get the sum of these digits
// We get the sum of the odd digits
// If the sum of both results in a number such that the first digit is 0,
// Then the credit card is valid. Otherwise false.

int checksum(long cc)
{
    int even_sum = 0;
    int odd_sum = 0;
    int digit;

    for (int i = 1; cc > 0; i++)
    {
        digit = cc % BASE;

        if (i % 2 == 0)
        {
            digit = digit * 2;
            digit = (digit > 9) ? digit % BASE + digit / BASE : digit;
            even_sum = even_sum + digit;
        }
        else
        {
            odd_sum = odd_sum + digit;
        }

        cc = cc / BASE;
    }

    if ((even_sum + odd_sum) % 10 == 0)
    {
        return 1;
    }

    return 0;
}

// AMEX: 15 digits, starts with 34 or 37
// MASTERCARD: 16 digits, starts with 51, 52, 53, 54 or 55
// VISA: 13 digits, starts with 4

int final_check(long cc)
{
    if (cc < 100)
    {
        printf("INVALID\n");
        return 0;
    }

    int count = 2;

    while (cc > 100)
    {
        count++;
        cc = cc / BASE;
    }

    printf("%d\n", count);

    if (count == 15 && (cc == 34 || cc == 37))
    {
        printf("AMEX\n");
    }
    else if ((count == 16 && cc == 51) || cc == 52 || cc == 53 || cc == 54 || cc == 55)
    {
        printf("MASTERCARD\n");
    }
    else if (count == 13 || (count == 16 && (cc / 10 == 4)))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}
