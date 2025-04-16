#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

// compile with:
// gcc xxx.c -lm -o xxx

#define width 80
#define height 44

float A, B, C;
const float cubeWidth = 40;
float zBuffer[width * height];
char buffer[width * height];

int backgroundASCII = ' ';
int cameraDistance = 80;
float K1 = 40;
float x, y, z;
float ooz;
int xp, yp;
int idx;

float calX(float i, float j, float k)
{
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calY(float i, float j, float k)
{
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}

float calZ(float i, float j, float k)
{
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calForPoint(float i, float j, float k, int ch)
{
    x = calX(i, j, k);
    y = calY(i, j, k);
    z = calZ(i, j, k) + cameraDistance;
    ooz = 1 / z;
    xp = (int)(width / 2 + K1 * ooz * x * 2);
    yp = (int)(height / 2 - K1 * ooz * y);
    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height)
    {
        if (ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main(int argc, char const *argv[])
{
    printf("\x1b[2J");
    while (1)
    {
        memset(buffer, backgroundASCII, width * height);
        memset(zBuffer, 0, width * height * sizeof(float));
        for (float i = -cubeWidth / 2; i < cubeWidth / 2; i += 0.15)
        {
            for (float j = -cubeWidth / 2; j < cubeWidth / 2; j += 0.15)
            {
                calForPoint(i, j, -cubeWidth / 2, '@');
                calForPoint(cubeWidth / 2, j, i, '*');
                calForPoint(-cubeWidth / 2, j, -i, '*');
                calForPoint(-i, j, cubeWidth / 2, '@');
                calForPoint(i, -cubeWidth / 2, -j, '.');
                calForPoint(i, cubeWidth / 2, j, '.');
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < width * height; k++)
        {
            putchar(k % width ? buffer[k] : 10);
        }
        A += 0.1;
        B += 0.1;
        C += 0.04;
        usleep(500);
    }
    return 0;
}