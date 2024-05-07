//programme qui récup une image 256 couleurs (palette N&B) déjà faite et la modifie.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 20
#define HEIGHT 15

#pragma pack(push, 1)
typedef struct {
    uint16_t type; //doit être égale à "BM" ou 0x424D
    uint32_t size; //taille totale du fichier pour image 10x7 = (10+2)*7*3 + 54 (taille du header) pour une image RGB
    // pour une image en N&B il faudra 54 + 256*4 (taille de la palette) + (10+2)*7
    uint16_t reserved1; // 0
    uint16_t reserved2; // 0
    uint32_t offset;//adresse du début des données, si palette de couleur, cet offset est égal à 40 + 256*4 et sinon égal à 40
    uint32_t dib_header_size; //taille de l'entête 54 ou 0x36
    uint32_t width; // largeur de l'image (multiple de 32 bits) donc 10 pixels et 2 pixels de paddings = 12 pixels
    uint32_t height; //hauteur de l'image 7 pixels
    uint16_t planes; // nombre de plans 1 
    uint16_t bits_per_pixel;// 8 ou 24
    uint32_t compression;//0
    uint32_t image_size;//10*7
    uint32_t x_resolution;//nbpixel/m sur x : 2835
    uint32_t y_resolution;//nbpixel/m sur y : 2835
    uint32_t colors_used;// 0 en RGB ou 256 en N&B sur 8 bits (codage 256 couleurs)
    uint32_t important_colors; // 0
} BMPHeader;
#pragma pack(pop)


int _calcul_padding(int largeur)
{
    int padding = 0;
    int largeur_ligne = largeur;
    if (largeur_ligne % 4 != 0)
        //on cherche l'écart pour avoir toujours 4 octets
        padding = 4 - (largeur_ligne % 4);
    return padding;
}
int _calcul_taille_ligne(int largeur)
{
    return (_calcul_padding(largeur) + largeur);
}
int _calcul_taille_image(int largeur, int hauteur)
{
    return(_calcul_taille_ligne(largeur) * hauteur);
}
int _calcul_taille_fichier(int largeur, int hauteur)
{
    return(0x36 + _calcul_taille_image(largeur, hauteur));
}

void view_header(BMPHeader header) {
    printf("Type de bitmap\n");
    printf("%x\n", header.type);
    printf("Taille de bitmap on octets:\n");
    printf("%d\n", header.size);
    printf("reserved1 :\n");
    printf("%d\n", header.reserved1);
    printf("reserved2 :\n");
    printf("%d\n", header.reserved2);
    printf("Data offset de la bitmap\n");
    printf("%d\n", header.offset);
    printf("Taille de l'entete:\n");
    printf("%d\n", header.dib_header_size);
    printf("Largeur de l'image:\n");
    printf("%d\n", header.width);
    printf("Hauteur de l'image:\n");
    printf("%d\n", header.height);
    printf("Nombre de planes:\n");
    printf("%d\n", header.planes);
    printf("Bits par pixels:\n");
    printf("%d\n", header.bits_per_pixel);
    printf("Mode de compression:\n");
    printf("%d\n", header.compression);
    printf("Taille de l'image:\n");
    printf("%d\n", header.size);
    printf("Xpelspermeter:\n");
    printf("%d\n", header.x_resolution);
    printf("Ypelspermeter:\n");
    printf("%d\n", header.y_resolution);
    printf("Couleur utilise:\n");
    printf("%d\n", header.colors_used);
    printf("Coleur importants:\n");
    printf("%d\n", header.important_colors);
}

void create_header(BMPHeader *header) {
    header->type = 0x4d42;
    header->size = _calcul_taille_fichier(WIDTH, HEIGHT);
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset = 1078;//54(header) + 256*4(palette)
    header->dib_header_size = 40;
    header->width = WIDTH;
    header->height = HEIGHT;
    header->planes = 1;
    header->bits_per_pixel = 8;
    header->compression = 0;
    header->image_size = _calcul_taille_image(WIDTH, HEIGHT);
    header->x_resolution = 2835;
    header->y_resolution = 2835;
    header->colors_used = 0;
    header->important_colors = 0;
}
void write_palette(FILE* file) {
    uint8_t zero = 0;
    for (int i = 0; i < 256; i++) {
        fwrite(&i, 1, 1, file);
        fwrite(&i, 1, 1, file);
        fwrite(&i, 1, 1, file);
        fwrite(&zero, 1, 1, file);
    }
}

int main() {
    FILE* file = fopen("..\\ressources\\mon_image.bmp", "wb");
    BMPHeader header;
    uint8_t data[HEIGHT][WIDTH];
    create_header(&header);
    fwrite(&header, sizeof(header), 1, file);
    view_header(header);
    write_palette(file);
//création de la matrice
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            data[i][j] = ((j * 15) > 255) ? 255 : (j * 15);
        }
    }
//écriture de la matrice
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fwrite(&data[i][j], 1, 1, file);
            printf("%3d ", data[i][j]);
        }
        fseek(file, (4 - (WIDTH % 4)) % 4, SEEK_CUR);
        printf("\n");
    }
    fclose(file);
    return 0;
}
