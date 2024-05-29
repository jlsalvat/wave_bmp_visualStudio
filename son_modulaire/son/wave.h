#pragma once //remplace #ifndef.... "endif
// wave.h
//#ifndef WAVE_H
//#define WAVE_H

void readWave(const char* fichier, uint8_t* header, uint8_t* data, int taille);
int readSizeWave(const char* fichier);
void writeWave(const char* fichier, uint8_t* header, uint8_t* data, int taille);


//#endif