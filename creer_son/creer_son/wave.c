#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>



#define T_ENTETE 44

/* Formats audios
 * (je ne connais que le PCM ...) */
enum F_AUDIO {
	PCM = 1
};

/* Taille de l'en-t�te subTaille1 */
enum T_AUDIO {
	T_PCM = 16
};

/* Canaux :
 *  Mono = 1 seule enceinte
 *  St�r�o = 2 enceintes
 *  GDC = 3 enceintes (gauche, droite, centre)
 *  GD_GD = 4 enceintes (avant gauche et droite, arri�re gauche et droite)
 *  GDCS = 5 enceintes (GDC + Surround)
 *  All = 6 enceintes (GDC, centre gauche, centre droit, surround) */
enum CAN {
	MONO = 1,
	STEREO = 2,
	GDC = 3,
	GD_GD = 4,
	GDCS = 5,
	ALL = 6
};

/* Fr�quence d'�chantillonnage
 * Valeurs en hz*/
enum F_ECH {
	F_8K = 8000,
	F_11K = 11025,
	F_22K = 22050,
	F_41K = 44100,
	F_48K = 48000, /* Encodage d'un CD audio  */
	F_96K = 96000
};

/* Bits par �chantillon */
enum BPE {
	BPE_8 = 8,
	BPE_16 = 16,
	BPE_24 = 24,
	BPE_32 = 32
};

struct wave {
	/* Le mot "RIFF" */
	char riff[4];
	/* La taille du fichier - 8
	 * Ou subTaille2 + 44 - 8 */
	int32_t taille;
	/* Le mot "WAVE" */
	char wave[4];
	/* Le mot "fmt " */
	char fmt[4];
	/* Taille du header jusqu'� "data" */
	int32_t subTaille1;
	/* Format du fichier */
	int16_t formatAudio;
	/* Nombres de canaux */
	int16_t nombreCanaux;
	/* Fr�quence d'�chantillonnage */
	int32_t freqEch;
	/* ByteRate
	 * Nombre d'ocets par seconde
	 ** Calcul :
	 *===> freqEch * nombreCanaux * bitsParEch / 8 */
	int32_t ByteRate;
	/* Alignement
	 * Nombre d'octets par �chantillon
	 * Englobe tous les canaux !
	 ** Calcul :
	 *===> nombreCanaux * bitsParEch / 8 */
	int16_t align;
	/* Bits par �chantillon */
	int16_t bitsParEch;

	/* Le mot "data" et la
	 * taille des donn�es */
	char Ndata[4];
	/* Taille des donn�es */
	int32_t subTaille2;

	/* A allouer dynamiquement.
	 * Contiendra les donn�es */
	void *data;
};

//variable globale
FILE *fich ;
struct wave wav = { 0 };

int wave_init(int nb_canaux,int nb_sec) {
	/* Les constantes symboliques */
	strncpy(wav.riff, "RIFF", 4);
	strncpy(wav.wave, "WAVE", 4);
	strncpy(wav.fmt, "fmt ", 4);
	strncpy(wav.Ndata, "data", 4);

	wav.subTaille1 = T_PCM;
	wav.formatAudio = PCM;
	wav.nombreCanaux = nb_canaux;
	wav.freqEch = F_8K;
	wav.bitsParEch = BPE_8;
	wav.ByteRate = wav.freqEch * wav.nombreCanaux * (wav.bitsParEch / 8);
	wav.align = wav.nombreCanaux * (wav.bitsParEch / 8);

	/* On peut calculer subTaille2 en fonction du byterate. Ce qui nous donne : */
	wav.subTaille2 = wav.ByteRate * nb_sec;

	/* On calcule la taille du fichier */
	wav.taille = wav.subTaille2 + 44 - 8;

	/* On alloue la partie donn�es */
	wav.data = malloc(wav.subTaille2);
	return wav.subTaille2;
}

void wave_create_file(char nomFichier[],int nb_canaux, int nb_sec)
{
	fich = fopen(nomFichier, "wb+");

	if (fich == NULL){
        printf("impossible de creer le fichier %s\n FIN DU PROG \n",nomFichier);
		exit(EXIT_FAILURE);
	}
	else {
		printf("creation du fichier %s\n", nomFichier);
		//init de la structure de donn�es associ�e au fichier wav
		wave_init(nb_canaux, nb_sec);
	}
}
void wave_save_file(unsigned char son[],int taille){
    //on copie le contenu du tableau son dans wav.data
    memcpy( wav.data, son, taille );
	/* On �cris l'en-t�te SANS les donn�es */
	printf("copie de l'entete\n");
	fwrite(&wav, T_ENTETE, 1, fich);
	/* On �cris les data */
	printf("copie des donnees : %d echantillons\n", taille);
	fwrite(wav.data, taille, 1, fich);
    printf("enregistrement et fermeture du fichier");
	/* On lib�re notre m�moire, et on ferme le fichier */
	free(wav.data);
	fclose(fich);
}

