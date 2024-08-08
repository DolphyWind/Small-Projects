#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#define SIZE (USHRT_MAX + 1)

unsigned short bitflip(unsigned short l)
{
    int bit_to_flip = rand() % 16;
    unsigned short mask = (1 << bit_to_flip);

    return (l ^ mask);
}

// Calculate string length by utilizing cosmic ray bit flipping
// Recommended to run in space. Scroll down for more detailed description
int find_length(char* str)
{
    unsigned short len = 0;

    while(1)
    {
        if(str[len] == 0)
            break;

        len = bitflip(len);
    }

    return len;
}

int main()
{
    srand(time(NULL));
    char input[SIZE];
    memset(input, -1, SIZE);
    scanf("%[^\n]", input);

    int len = find_length(input);
    printf("The length of the string is: %d", len);

    return 0;
}
