// pour pouvoir utiliser fopen
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void readWave(const char* fichier, uint8_t* header, uint8_t* data, int taille) {
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
    fread(data, taille, 1, file);
    fclose(file);
}
int readSizeWave(const char* fichier) {
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
void writeWave(const char* fichier, uint8_t* header, uint8_t* data, int taille) {
    FILE* file;
    // Ouvrir le fichier en mode écriture binaire
    file = fopen(fichier, "wb");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    fwrite(header, 44, 1, file);
    fwrite(data, taille, 1, file);
    fclose(file);
}