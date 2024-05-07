#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"

#define LARGEUR 10
#define HAUTEUR 7
int main()
{
    //création de fichier bmp avec un motif
    unsigned char R[HAUTEUR][LARGEUR];
    unsigned char G[HAUTEUR][LARGEUR];
    unsigned char B[HAUTEUR][LARGEUR];
    bmp_create_file("shape.bmp",LARGEUR,HAUTEUR);
    for(int i=0; i<HAUTEUR; i++)
    {
        for(int j=0; j<LARGEUR; j++)
        {
            R[i][j]=0;
            G[i][j]=0;
            B[i][j]=127;
        }
    }
    bmp_save_file(R,G,B);

    //copie de fichier binaire
    bmp_copy_file("tweet_back.bmp","tweet.bmp");

    //ouvrir un fichier bmp et le modifier
    int largeur,hauteur;
    bmp_open_file("tweet.bmp", &largeur, &hauteur);
    //création des 3 tableaux R,G,B, seule solution possible pour les 3 matrices
    unsigned char *R1= (unsigned char *) malloc(largeur*hauteur);
    unsigned char G1[LARGEUR * HAUTEUR]; //VLA
    unsigned char *B1=(unsigned char *) malloc(largeur*hauteur);
    //mise à jour des 3 matrices RGB
    bmp_read_RGB(R1,G1,B1);
    //inversion de l'image
    for(int i=0; i<hauteur; i++)
    {
        for(int j=0; j<largeur; j++)
        {
            R1[i*largeur+j]=255-R1[i*largeur+j];
            G1[i*largeur+j]=255-*(G1+i*largeur+j);
            B1[i*largeur+j]=255-*(B1+i*largeur+j);
        }
    }
    //enregistrement de l'image
    bmp_save_file(R1,G1,B1);

    return 0;
}
