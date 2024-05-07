#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define TAILLE_PIXEL_BIT 24
#define TAILLE_PIXEL_OCTET TAILLE_PIXEL_BIT/8
#define ADD_IMAGE 0x36
#define TAILLE_header_HEADER 0x28
#define RESOLUTION_PX 0x2E23

struct header
{
//taille totale du fichier
    int32_t taille_fichier;
// doit être à 0
    int32_t reserve;
// addresse du début de l'image=0x36
    int32_t add_image;
// taille header Header = 0x28"
    int32_t taille_header_header;
// Largeur image
    int32_t largeur;
//hauteur image
    int32_t hauteur;
//nb_plans=1
    int16_t nb_plans;
//taille pixel=24 bits
    int16_t taille_pixel;
//compression=0 pas de compression
    int32_t compression;
//taille de l'image (multiples de 4 octets)
    int32_t taille_image;
//resolution_px_h=0x2E23
    int32_t resolusion_px_h;
//resolution_px_v=0x2E23
    int32_t resolusion_px_v;
//nb_couleurs=0
    int32_t nb_couleurs;
//nb_couleurs_i=0
    int32_t nb_couleurs_i;
};

//variable globale bmp
const char tag[] = { 'B', 'M' };
struct header header = { 0 };
unsigned char  *data;

//autres variables globales
FILE *fich ;
int padding;

int _calcul_padding(int largeur)
{
    int padding=0;
    int largeur_ligne=largeur*TAILLE_PIXEL_OCTET;//on multiplie par 3 B,V,R
    if( largeur_ligne % 4 != 0 )
        //on cherche l'écart pour avoir toujours 4 octets
        padding= 4 - (largeur_ligne%4);
    return padding;
}
int _calcul_taille_ligne(int largeur)
{
    return (_calcul_padding(largeur)+largeur*TAILLE_PIXEL_OCTET);
}
int _calcul_taille_image(int largeur, int hauteur)
{
    return(_calcul_taille_ligne(largeur)*hauteur);
}

int _calcul_taille_fichier(int largeur,int hauteur)
{
    return(ADD_IMAGE+_calcul_taille_image(largeur,hauteur));
}

int _validation_tags()
{
    int count=sizeof(tag);
    char buffer[2];
    fseek(fich, 0, SEEK_SET);
    int n=fread( buffer, 1, count, fich);
    if ( n!=2 )
    {
        printf(" ERREUR lecture fichier\n");
        return EXIT_FAILURE;
    }
    if (memcmp(buffer,tag,2)!=0)
    {
        printf(" ERREUR fichier non BMP\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS; //ok
}

void _init_header(int largeur, int hauteur)
{
    /* Les constantes symboliques */
    header.taille_fichier=_calcul_taille_fichier(largeur,hauteur);
    header.reserve=0;
    header.add_image=ADD_IMAGE;
    header.taille_header_header=TAILLE_header_HEADER;
    header.largeur=largeur;
    header.hauteur=hauteur;
    header.nb_plans=1;
    header.taille_pixel=TAILLE_PIXEL_BIT;
    header.compression=0;
    header.taille_image=_calcul_taille_image(largeur,hauteur);
    header.resolusion_px_h=RESOLUTION_PX;
    header.resolusion_px_v=RESOLUTION_PX;
    header.nb_couleurs=0;
    header.nb_couleurs_i=0;
}
void _read_header()
{
    //Lecture du header
    fread(&header, sizeof(header), 1, fich);
    if(header.taille_pixel!=TAILLE_PIXEL_BIT)
    {
        printf(" ERREUR : Ce n'est pas un fichier BMP 24 bits\n");
        exit(EXIT_FAILURE);
    }
}

void bmp_copy_file(char nomFichierDest[],char nomFichierSrc[])
{
    unsigned char * buffer=NULL;
    FILE * source=NULL;
    FILE * copy=NULL;
    printf("copie de %s vers %s\n",nomFichierSrc,nomFichierDest);
    source=fopen(nomFichierSrc,"rb");
    if(source ==NULL ){
        printf(":fichier %s n'existe pas",nomFichierSrc);
        return 0;
    }
    copy=fopen(nomFichierDest,"wb");
    if(copy == NULL){
        printf(":creation du fichier %s a echoue \n",nomFichierDest);
        fclose(source);
        return 0;
    }
    fseek(source,0,SEEK_END);
    int size=ftell(source);
    printf("\n size=%d\n", size);
    rewind(source);
    buffer=malloc(size*sizeof(unsigned char));

    fread(buffer, size * sizeof(unsigned char), 1, source);
    fwrite(buffer, size * sizeof(unsigned char), 1, copy);
//read(buffer,sizeof(unsigned char),size,source);
//    write(buffer,sizeof(unsigned char),size,copy);

    fclose(source);
    fclose(copy);

    free(buffer);

}

void bmp_create_file(char nomFichier[],int largeur, int hauteur)
{
    fich = fopen(nomFichier, "wb+");

    if (fich == NULL)
    {
        printf("impossible de creer le fichier %s\n FIN DU PROG \n",nomFichier);
        exit(EXIT_FAILURE);
    }
    else
        printf("creation du fichier %s\n", nomFichier);
    //init de la structure de données associée au fichier wav
    _init_header(largeur, hauteur);
    /* On alloue la partie données */
    data = (unsigned char *) malloc(header.taille_image);
}
void bmp_open_file(char nomFichier[], int *largeur, int *hauteur)
{
    fich = fopen(nomFichier, "rb+");

    if (fich == NULL)
    {
        printf("impossible d'ouvrir le fichier %s\n FIN DU PROG \n",nomFichier);
        exit(EXIT_FAILURE);
    }
    else
        printf("ouverture du fichier %s\n", nomFichier);
    int validation_fichier= _validation_tags()==0;
    if(validation_fichier)
    {
        //Mise à jour de la structure header
        _read_header();
        //copie des données
        data = (unsigned char *) malloc(header.taille_image);
        fread( data, header.taille_image, 1, fich );
        *largeur=header.largeur;
        *hauteur=header.hauteur;
    }
    else
        exit(EXIT_FAILURE);
}

void bmp_read_RGB(unsigned char *R,unsigned char *G,unsigned char *B)
{

    int i=0,j=0,k=0;
    while(k<header.taille_image)
    {
        *(B+i*header.largeur+j)=data[k++];
        *(G+i*header.largeur+j)=data[k++];
        *(R+i*header.largeur+j)=data[k++];
        j++;
        if((j)%header.largeur==0)
        {
            k=k+_calcul_padding(header.largeur);
            i++;
            j=0;
        }
    }
}

void bmp_save_file(unsigned char *R,unsigned char *G,unsigned char *B)
{
    int padding = _calcul_padding(header.largeur);
    int k=0;
    for(int i=0; i<header.hauteur; i++)
    {
        for(int j=0; j<header.largeur; j++)
        {
            data[k++]=*(B+i*header.largeur+j);
            data[k++]=*(G+i*header.largeur+j);
            data[k++]=*(R+i*header.largeur+j);
        }
        for(int j=0; j<padding; j++)
            data[k++]=0;
    }
    printf("copie de l'entete\n");
    //on se remet au début du fichier
    fseek(fich, 0, SEEK_SET);
    fwrite(&tag, sizeof(tag), 1, fich);
    fwrite(&header, sizeof(header), 1, fich);
    /* On écris les data */
    printf("copie des donnees : %d echantillons\n", header.taille_image);
    fwrite(data, header.taille_image, 1, fich);
    printf("enregistrement et fermeture du fichier\n");
    /* On libère notre mémoire, et on ferme le fichier */
    free(data);
    fclose(fich);
}
