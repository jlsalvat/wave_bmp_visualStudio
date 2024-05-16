#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include "wave.h"

#define FREQ_NOTE_EN_HERTZ  250 // partie paramétrable pour générer une sinuoide
#define OMEGA   2 * M_PI * FREQ_NOTE_EN_HERTZ  //calcul de omega pour la sinusoide.

#define NB_SEC 3 //nombres de secondes de notre fichier wav
#define FREQ 8000
#define NB_ECHANTILLONS FREQ*NB_SEC 





int main(void)
{
    double t = 0 ;//temps
    double phi=0;//dephasage
    wave_create_file("sinus.wav",1,NB_SEC); //ouverture du fichier
    unsigned char data[NB_ECHANTILLONS];
    for (int i = 0; i < 20; i++)
    {
        data[i] = 127;
    }
    //génération de la sinusoide
    for (int i=20 ; i< NB_ECHANTILLONS/2+4; i++)
    {
        data[i] = 127 + 127 * sin(OMEGA*t+phi) ;
        t = t + 1.0/ FREQ;//Freq échantillonnage 8kHz
    }
    for (int i = NB_ECHANTILLONS / 2+4 ; i < NB_ECHANTILLONS ; i++)
    {
        data[i] = 127 ;
    }
    //sauvegarde du fichier avec les données créées.
    wave_save_file(data, NB_ECHANTILLONS);
    return 0;
}

