// pour pouvoir utiliser fopen
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    FILE* file;
    // Ouvrir le fichier en mode lecture & modification binaire
    file = fopen("..\\ressources\\sinus.wav", "rb+");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }
    // Aller à la position de début des données audio (44 octets)
    fseek(file, 44, SEEK_SET);

    unsigned char tab[1024];
    // Lire 1024 octets dans le fichier et les placer dans le tableau
    fread(tab, 1, 1024, file);

    // Afficher les données audio (fichier en mono et sur 8 bits)
    for (int i = 0; i < 1024; i++) {
        printf("%hhu\n", tab[i]);
    }
    fseek(file, 44, SEEK_SET);
    for (int i = 0; i < 10; i++) {
        tab[i] = 127;
    }
    fwrite(tab, 1, 1024, file);
    // Fermer le fichier
    fclose(file);

    return 0;
}