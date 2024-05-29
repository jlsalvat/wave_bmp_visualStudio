// pour pouvoir utiliser fopen
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define DO 277.18
#define RE 311.13
#define MI 329.63
#define FA 349.23
#define SOL 392.00
#define LA 440.00
#define SI 493.88
#define DO_C5 523.25

int readSizeWave(char* fichier) {
    FILE* file;
    int taille;
    // Ouvrir le fichier en mode lecture modification binaire
    file = fopen(fichier, "rb");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return -1;
    }
    // Aller à la position de la taille des datas (40 octets)
    fseek(file, 40, SEEK_SET);
    // Lire la taille du tableau
    fread(&taille, sizeof(int), 1, file);
    fclose(file);
    return taille;
}
void readDataWave(char* fichier, uint8_t *header, uint8_t* data, int taille) {
    FILE* file;
    // Ouvrir le fichier en mode lecture binaire
    file = fopen(fichier, "rb");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    //Lire le header
    fread(header, 1, 44, file);
    // Lire toutes les données du tableau
    fread(data, taille,1 , file);
    fclose(file);
}
void displayData(uint8_t data[], int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%hhu\n", data[i]);
    }
}
void WriteDataWave(char* fichier, uint8_t* header, uint8_t* data, int taille) {
    FILE* file;
    // Ouvrir le fichier en mode écriture binaire
    file = fopen(fichier, "wb");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    fwrite(header, 44, 1, file);
    fwrite(data, taille,1 , file);
    fclose(file);
}
void changeDataWave(uint8_t* data, int debut, int fin, double freq) {
    for (int i = debut; i < fin; i++)
        data[i] = 127 + 80 * sin(2 * M_PI / 8000.0 * freq * (i - debut));
}

int main() {
    uint8_t header[44];
    uint8_t* data;

    int taille = readSizeWave("..\\ressources\\sinus.wav");
    data = malloc(taille);
    readDataWave("..\\ressources\\sinus.wav", header,data,taille);
    changeDataWave(data, 0, 2000, DO);
    changeDataWave(data, 2001, 4000, RE);
    changeDataWave(data, 4001, 6000, MI);
    changeDataWave(data, 6001, 8000, FA);
    changeDataWave(data, 8001, 10000, SOL);
    changeDataWave(data, 10001, 12000, LA);
    changeDataWave(data, 12001, 14000, SI);
    changeDataWave(data, 14001, 20000, DO_C5);
    displayData(data, 200);
    WriteDataWave("..\\ressources\\sinus_back.wav", header, data,taille);
    free(data);
    return 0;
}