#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE 512
typedef uint8_t BYTE;

int cond(BYTE buffer[SIZE]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE* mcard = fopen(argv[1], "r");

    if (mcard == NULL)
    {
        printf("FIle does not exist.\n");
        fclose(mcard);
        return 1;
    }

    BYTE buffer[SIZE];

    FILE* img = NULL;
    char filename[8];
    int counter = 0;

    while (fread(buffer, sizeof(BYTE) * SIZE, 1, mcard) == 1)
    {
        if (cond(buffer) == 1)
        {
            if (img != NULL)
            {
                fclose(img);
            }
            sprintf(filename, "%03d.jpg", counter++);
            img = fopen(filename, "w");
        }

        if (img != NULL)
        {
            fwrite(buffer, sizeof(BYTE) * SIZE, 1, img);
        }
    }

    if (img != NULL)
    {
        fclose(img);
    }
    fclose(mcard);

    return 0;
}

int cond(BYTE buffer[SIZE])
{
    if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && buffer[3] >= 0xE0 && buffer[3] <= 0xEF)
    {
        return 1;
    }
    return 0;
}
