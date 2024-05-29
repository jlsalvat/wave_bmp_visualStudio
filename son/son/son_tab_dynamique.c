// pour pouvoir utiliser fopen
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//lecture du fichier sinus.wav, lecture de la taille des donn�es et
//cr�ation du tableau dynamique de la taille des donn�es, modification et enregistrement
int main() {
    FILE* file;
    int taille;
    uint8_t * tab;
    // Ouvrir le fichier en mode lecture & modification binaire
    file = fopen("..\\ressources\\sinus.wav", "rb+");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }
    // Aller � la position de la taille des donn�es (40 octets)
    fseek(file, 40, SEEK_SET);
    // Lire la taille du tableau
    fread(&taille, sizeof(int), 1, file);
    //cr�er le tableau dynamique
    tab = malloc(taille);
    if (tab==NULL) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // Lire 1024 octets dans le fichier et les placer dans le tableau
    fread(tab, taille, 1, file);

    // Afficher les donn�es audio (fichier en mono et sur 8 bits)
    for (int i = 0; i < 200; i++) {
        printf("%hhu\n", tab[i]);
    }
    //modifier les 100 premiers �chantillons
    fseek(file, 44, SEEK_SET);
    for (int i = 0; i < 100; i++) {
        tab[i] = 127;
    }
    //Ecrire dans le fichier
    fwrite(tab, taille, 1, file);
    // Fermer le fichier
    fclose(file);
    //lib�rer la m�moire
    free(tab);

    return 0;
}