// pour pouvoir utiliser fopen
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "wave.h"

#define DO 277.18
#define RE 311.13
#define MI 329.63
#define FA 349.23
#define SOL 392.00
#define LA 440.00
#define SI 493.88
#define DO_C5 523.25

//affiche le tableau
void displayData(uint8_t data[], int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%hhu\n", data[i]);
    }
}
//génère une sinusoide de frequence freq pour un fichier wave à 8kHz
void changeDataWave(uint8_t* data, int debut, int fin, double freq) {
    for (int i = debut; i < fin; i++)
        data[i] = 127 + 80 * sin(2 * M_PI / 8000.0 * freq * (i - debut));
}

int main() {
    uint8_t header[44];
    uint8_t* data;
    int taille = readSizeWave("..\\ressources\\sinus.wav");
    data = malloc(taille);
    readWave("..\\ressources\\sinus.wav", header,data,taille);
    changeDataWave(data, 0, 2000, DO);
    changeDataWave(data, 2001, 4000, RE);
    changeDataWave(data, 4001, 6000, MI);
    changeDataWave(data, 6001, 8000, FA);
    changeDataWave(data, 8001, 10000, SOL);
    changeDataWave(data, 10001, 12000, LA);
    changeDataWave(data, 12001, 14000, SI);
    changeDataWave(data, 14001, 20000, DO_C5);
    displayData(data, 200);
    writeWave("..\\ressources\\sinus_back.wav", header, data,taille);
    free(data);
    return 0;
}