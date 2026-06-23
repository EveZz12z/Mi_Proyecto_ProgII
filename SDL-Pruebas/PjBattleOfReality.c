#include<stdio.h>
#include<string.h>

//Struct de Personaje

typedef struct personaje
{
	char Nombre[30];	//Nombre del pj
	int Atq;   		//Ataque del pj
	int Hp;			//Vida
	int Def;		//Defensa
	float PosX, PosY;	//Posicion de aparicion
	float VelX, VelY;	//Velocidades
	int H, W;	        //Para HitBox  H=Alto ; W=Ancho

}	Personaje;		//Creamos el "tipo" Personaje

void IniciarFeinryr(Personaje *p) //Puntero para que amarre la copia de pj
{

	strcpy(p->Nombre, "Feinryr");  //"->" reemplazamos los valores anteriores
	
	p->Atq = 3;
	p->Hp = 100;		   
	p->Def = 3;		 

	p->PosX = 16;
	p->PosY	= 22;

	p->VelX = 0.5;
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

int main() //borrar el main a futuro, este es de prueba
{

	Personaje feinryr;

	IniciarFeinryr(&feinryr);
	MostrarEstadisticas(feinryr);

return 0;
}
