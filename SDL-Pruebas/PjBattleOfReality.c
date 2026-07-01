#include<stdio.h>
#include<string.h>

//Structfeeefff  

//Struct de Personaje

typedef struct personaje
{
	char Nombre[30];//Nombre del pj
	int Vidas;      //Cantidad de vidas
	int Atq;   		//Ataque del pj
	int Hp;			//Vida
	int HpMax;		//Vida Maxima
	int Def;		//Defensa
	float PosX, PosY;	//Posicion de aparicion
	int VelX;
	float VelY;	//Velocidades
	int H, W;	        //Para HitBox  H=Alto ; W=Ancho

}	Personaje;		//Creamos el "tipo" Personaje

void IniciarFeinryr(Personaje *p) //Puntero para que amarre la copia de pj
{

	strcpy(p->Nombre, "Feinryr");  //"->" reemplazamos los valores anteriores
	
	p->Vidas = 1;
	p->Atq = 3;
	p->Hp = 100;
	p->HpMax = 100;		   
	p->Def = 3;		 

	p->PosX = 22; //Columna
	p->PosY	= 16; //Fila

	p->VelX = 5;
	p->VelY = 0.5;

	p->H = 2;
	p->W = 1;
}

void IniciarAksha(Personaje *p)
{

	strcpy(p->Nombre, "Aksha");  //"->" reemplazamos los valores anteriores
	
	p->Vidas = 1;
	p->Atq = 4;
	p->Hp = 100;
	p->HpMax = 100;		   
	p->Def = 2;		 

	p->PosX = 39; //Columna
	p->PosY	= 16; //Fila

	p->VelX = 6;
	p->VelY = 0.5;

	p->H = 2;
	p->W = 1;
}

void MostrarEstadisticas(Personaje p)
{
	printf("==Pj==\n");
	printf("Nombre: %s\n", p.Nombre);
	printf("Hp:%d\n", p.Hp);
	printf("Def:%d\n", p.Def);
}
