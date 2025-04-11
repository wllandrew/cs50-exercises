#include <stdio.h>
#include <cs50.h>

void bricks(int tiles);

int main(void) {

    int tiles;

    do {
        tiles = get_int("Height: ");
    } while (tiles < 1);

    bricks(tiles);
}

void bricks(int tiles) {

    for (int i = 1; i <= tiles; i++) {
        for (int j = 1; j <= tiles - i; j++) {
            printf(" ");
        }
        for (int j = 1; j <= i; j++) {
            printf("#");
        }

        printf("  ");

        for (int j = i; j != 0; j--) {
            printf("#");
        }
        printf("\n");
    }

}
