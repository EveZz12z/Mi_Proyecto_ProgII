#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<string.h>

#define FILAS 30
#define COLUMNAS 60

//Struct de los Pj

typedef struct personaje
{

	char Nombre[30];
	int Hp;
	int Def;

	float PosX, PosY; //Las celdas donde se ubicara
	float VelX, VelY; //Las vel que tendra

	int Alto, Ancho;
} Personaje;

void IniciarFeinryr(Personaje *p)
{
	strcpy()
}
