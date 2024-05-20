void bmp_create_file(char nomFichier[],int largeur, int hauteur);

void bmp_save_file(unsigned char *R,unsigned char *G,unsigned char *B);

void bmp_copy_file(char nomFichierDest[],char nomFichierSrc[]);

void bmp_open_file(char nomFichier[], int *largeur, int * hauteur);

void bmp_read_RGB(unsigned char *R,unsigned char *G,unsigned char *B );
