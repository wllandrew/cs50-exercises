#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr = image[i][j];
            BYTE avg = round((curr.rgbtBlue + curr.rgbtGreen + curr.rgbtRed) / 3.0);
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = avg;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int wm = (width % 2 == 0) ? width / 2 : floor(width / 2);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < wm; j++)
        {
            RGBTRIPLE swap = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = swap;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temporary[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blueCount = 0;
            int redCount = 0;
            int greenCount = 0;
            int cellCount = 0;

            for (int n = -1; n < 2; n++)
            {
                for (int m = -1; m < 2; m++)
                {
                    if (i + n < 0 || i + n >= height || j + m < 0 || j + m >= width)
                    {
                        continue;
                    }

                    RGBTRIPLE iteration = image[i + n][j + m];
                    blueCount += iteration.rgbtBlue;
                    greenCount += iteration.rgbtGreen;
                    redCount += iteration.rgbtRed;
                    cellCount++;
                }
            }

            temporary[i][j].rgbtRed = round(redCount / (float) cellCount);
            temporary[i][j].rgbtBlue = round(blueCount / (float) cellCount);
            temporary[i][j].rgbtGreen = round(greenCount / (float) cellCount);

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temporary[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temporary[height][width];
    int gx[3][3] =   {{-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};
    int gy[3][3] =   {{-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int bluecountX = 0;
            int redcountX = 0;
            int greencountX = 0;

            int bluecountY = 0;
            int redcountY = 0;
            int greencountY = 0;

            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {

                    if (i + m < 0 || i + m > height - 1 || j + n < 0 || j + n > width - 1)
                    {
                        continue;
                    }

                    RGBTRIPLE curr = image[i + m][j + n];

                    bluecountX += curr.rgbtBlue * gx[m + 1][n + 1];
                    greencountX += curr.rgbtGreen * gx[m + 1][n + 1];
                    redcountX += curr.rgbtRed * gx[m + 1][n + 1];

                    bluecountY += curr.rgbtBlue * gy[m + 1][n + 1];
                    greencountY += curr.rgbtGreen * gy[m + 1][n + 1];
                    redcountY += curr.rgbtRed * gy[m + 1][n + 1];
                }
            }
            int blue = (int) round(sqrt(bluecountX*bluecountX + bluecountY*bluecountY));
            int green = (int) round(sqrt(greencountX*greencountX + greencountY*greencountY));
            int red = (int) round(sqrt(redcountX*redcountX + redcountY*redcountY));

            temporary[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
            temporary[i][j].rgbtGreen = (green > 255) ? 255 : green;
            temporary[i][j].rgbtRed = (red > 255) ? 255 : red;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temporary[i][j];
        }
    }

    return;
}
