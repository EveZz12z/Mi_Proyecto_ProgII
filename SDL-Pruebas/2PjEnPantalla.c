#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "PjBattleOfReality.c" //Para poder ingresar a feinryr y aksha


#define FILAS 30
#define COLUMNAS 60


//Para mis bloques solidos

typedef struct
{
	SDL_Rect rect;
	int tipo; //1-5 spritesheets , 6 = bloque dañino
	
} Bloque;

//Funcion para mi barra de vida

void DibujarBarraVida(SDL_Renderer *renderer, Personaje p, int x)
{
	int porcentaje = (p.Hp * 100) / p.HpMax;
	int ancho = porcentaje * 2; //Maximo 200 pixeles
	
	SDL_Rect barra = {x, 20, ancho, 20 };
	SDL_Rect borde = {x, 20, 200, 20};

	if(porcentaje >60)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //Verde
	}
	else if(porcentaje > 30)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //Amarillo
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //Rojo
	}

	SDL_RenderFillRect(renderer, &barra);      			  // Rellena la barra
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &borde);      			  // Dibuja el borde
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	int mapa[FILAS][COLUMNAS];
	int i, j;

	//variables para bloques solidos
	int TotalBloques = 0;
	int valor;

	//gravedad
	float Gravedad = 0.5;

	//para que el juego funcione
	int corriendo = 1;

	//pantalla
	int screenWidth = 1920;
	int screenHeight = 1080;

	int BLOQUE_ANCHO = screenWidth / COLUMNAS;
	int BLOQUE_ALTO = screenHeight / FILAS;

	SDL_Window *ventana = SDL_CreateWindow("Battle of Reality",
					  						SDL_WINDOWPOS_CENTERED,
					  						SDL_WINDOWPOS_CENTERED,
	       			  						screenWidth,
					  						screenHeight,
					  						0);

	SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture *fondo = IMG_LoadTexture(renderer, "1MapaBattleOfReality.png");

	SDL_Texture *sprite1 = IMG_LoadTexture(renderer, "Sprite4-Bloque1.png"); 
	SDL_Texture *sprite2 = IMG_LoadTexture(renderer, "Sprite4-Bloque2.png"); 
	SDL_Texture *sprite3 = IMG_LoadTexture(renderer, "Sprite4-Bloque3.png"); 
	SDL_Texture *sprite4 = IMG_LoadTexture(renderer, "Sprite4-Bloque4.png"); 
	SDL_Texture *sprite5 = IMG_LoadTexture(renderer, "Sprite5-Bloque1.png");

	FILE *archivo = fopen("1mapa30x60.txt", "r");

	for(i=0; i<FILAS;i++)
	{
		for(j=0; j<COLUMNAS;j++)
		{
			fscanf(archivo, "%1d", &mapa[i][j]);
		}
	}
	
	fclose(archivo);

	Bloque bloques[2000]; //En realidad son 1800 pero para no tener problemas lo dejamos en 2000 a secas

	for(i=0; i<FILAS;i++) //Recorremos todo el mapa en busca de los bloques colisionables
	{
		for(j=0; j<COLUMNAS;j++)
		{
			valor = mapa[i][j]; //Variable para guardar los valores del .txt

			if(valor >=1 && valor <=6) //Condiciones para que los bloques sean colisionables
			{
						bloques[TotalBloques].rect.x = j * BLOQUE_ANCHO; 	//Posiciones
		                bloques[TotalBloques].rect.y = i * BLOQUE_ALTO;  	//Posiciones
                		bloques[TotalBloques].rect.w = BLOQUE_ANCHO;	 	//Tamaño 32x36 pixeles
		                bloques[TotalBloques].rect.h = BLOQUE_ALTO;	 		//Tamaño  ""     ""

                		bloques[TotalBloques].tipo = valor; 		 		//Guardamos el tipo de bloque

		                TotalBloques++;					 					//Aumentamos la cantidad de bloques creados
			}
		}
	}

	//Iniciamos a Feinryr

	Personaje feinryr;							//Creamos variable (feinryr) de tipo Personaje
	IniciarFeinryr(&feinryr);					//Llamamos los valores asignados (hp,def,vel...etc)

	SDL_Rect pjF;								//Creamos el pj (rectangulo, aun no se dibuja)
	pjF.w = feinryr.W * BLOQUE_ANCHO;			//Tamaño del pj en pixeles W=1 Bloque_ancho = 32 entonces es de 32 pixeles
	pjF.h = feinryr.H * BLOQUE_ALTO;			//72 pixeles (2 bloques) h=2 bloque alto = 36
	pjF.x = feinryr.PosX * BLOQUE_ANCHO;		//Convertimos la posicion real en pixeles posX= 22 x 32 = 704 pixeles
	pjF.y = feinryr.PosY * BLOQUE_ALTO - pjF.h;	//posY = 16 x 36 = 576 pixeles

	float velY_F = 0;				//Para la gravedad
	int enSuelo_F = 0;
	int saltos_F = 0;				//Cuantos saltos a realizado 

	//Iniciamos a Aksha
	Personaje aksha;
	IniciarAksha(&aksha);

	SDL_Rect pjA;
	pjA.w = aksha.W * BLOQUE_ANCHO;
	pjA.h = aksha.H * BLOQUE_ALTO;
	pjA.x = aksha.PosX * BLOQUE_ANCHO;
	pjA.y = aksha.PosY * BLOQUE_ALTO - pjA.h;

	float velY_A = 0;
	int enSuelo_A = 0;
	int saltos_A = 0;

	//Iniciamos el evento

	SDL_Event evento;						//Inicamos evento
	
	while(corriendo)						
	{
		while(SDL_PollEvent(&evento))				//Se revisa si paso algun evento
		{
			if(evento.type == SDL_QUIT)			
			{
				corriendo = 0;				//El juego termina
			}
			if(evento.type == SDL_KEYDOWN && evento.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				corriendo = 0;
			}
			else if (evento.type == SDL_KEYDOWN && evento.key.repeat == 0)
            {
                //Salto de Feinryr con W
                if (evento.key.keysym.scancode == SDL_SCANCODE_W)
                {
                    if (enSuelo_F == 1)
                    {
                        velY_F = -10;
                        saltos_F = 1;   //primer salto
                        enSuelo_F = 0;
                    }
                    else if (saltos_F < 2)
                    {
                        velY_F = -10;
                        saltos_F++;     //segundo salto
                    }
                }

                //Salto de Aksha con UP
                if (evento.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    if (enSuelo_A == 1)
                    {
                        velY_A = -10;
                        saltos_A = 1;
                        enSuelo_A = 0;
                    }
                    else if (saltos_A < 2)
                    {
                        velY_A = -10;
                        saltos_A++;
                    }
                }
            }
		}
		const Uint8 *estado = SDL_GetKeyboardState(NULL);	//Guardamos el estado de todas las teclas

		//Movimiento de Feinryr + gravedad

		SDL_Rect nuevoF = pjF;					//Copiamos el rectangulo del pj

		velY_F += Gravedad;					//Aplicamos la gravedad por frame
		nuevoF.y += velY_F;					

		if(estado[SDL_SCANCODE_A])
		{
			nuevoF.x -= feinryr.VelX;
		}

		if(estado[SDL_SCANCODE_D])
		{
			nuevoF.x += feinryr.VelX;
		}
		
		//Movimiento de Aksha 

		SDL_Rect nuevoA = pjA;

		velY_A += Gravedad;
		nuevoA.y += velY_A;

		if(estado[SDL_SCANCODE_LEFT])
		{
			nuevoA.x -= aksha.VelX;
		}
		if(estado[SDL_SCANCODE_RIGHT])
		{
			nuevoA.x += aksha.VelX;
		}
	

		//Colisiones para Feinryr

		enSuelo_F = 0;											//Se asume que feinryr estara en el aire, si toca un bloque solido este valor cambiara a 1

		for(i=0; i<TotalBloques; i++)									//Recorremos todos los bloques del mapa 
		{
			if(SDL_HasIntersection(&nuevoF, &bloques[i].rect))					//En el caso de que feinryr este tocando algun bloque slido entrara dentro de este if
			{
				if(bloques[i].tipo == 6)							//En el caso de que sea el bloque 6 (bloque dañino) este le hara daño
				{
					feinryr.Hp -= 5;
					printf("HP Feinryr: %d%%\n", (feinryr.Hp * 100) / feinryr.HpMax);
				}

				if(feinryr.Hp <= 0)								//Si feinryr se queda sin vida se acabara el juego 
				{
					printf("Feinryr se ha quedado sin vida...\n");
					corriendo = 0;
				}

				if(velY_F > 0)									//Deteccion de ver si feinryr cae sobre un bloque (para que no traspase el suelo vaya)
				{
					nuevoF.y = bloques[i].rect.y - pjF.h;
					enSuelo_F = 1;
					saltos_F = 0;
				}

				velY_F = 0;									//Para que la gravedad no afecte y pueda volver a saltar
				break;										//se termina el bucle de colisiones
			}
		}

		//Colisiones para Aksha

		enSuelo_A = 0;

		for(i=0; i<TotalBloques; i++)
		{
			if(SDL_HasIntersection(&nuevoA, &bloques[i].rect))
			{
				if(bloques[i].tipo == 6)
				{
					aksha.Hp -= 5;
					printf("HP Aksha: %d%%\n", (aksha.Hp * 100) / aksha.HpMax);
				}

				if(aksha.Hp <= 0)
				{
					printf("Aksha se ha quedado sin vida...\n");
					corriendo = 0;
				}

				if(velY_A > 0)
				{
					nuevoA.y = bloques[i].rect.y - pjA.h;
					enSuelo_A = 1;
					saltos_A = 0;
				}

				velY_A = 0;
				break;
			}
		}
		
		//Copiamos ese rectangulo temporal al rectangulo real

		pjF = nuevoF;
		pjA = nuevoA;

		//Dibujamos todo

		SDL_RenderCopy(renderer, fondo, NULL, NULL);

		for(i=0; i<TotalBloques; i++)
		{
			if(bloques[i].tipo == 6)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &bloques[i].rect);
				continue;
			}

			SDL_Texture *tex = NULL;

			if(bloques[i].tipo == 1)
			{
				tex = sprite1;
			}
			if(bloques[i].tipo == 2)
			{
				tex = sprite2;
			}
			if(bloques[i].tipo == 3)
			{
				tex = sprite3;
			}
			if(bloques[i].tipo == 4)
			{
				tex = sprite4;
			}
			if(bloques[i].tipo == 5)
			{
				tex = sprite5;
			}

			SDL_RenderCopy(renderer, tex, NULL, &bloques[i].rect);
		}

		//Dibujamos a Feinryr

		SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
		SDL_RenderFillRect(renderer, &pjF);

		//Dibujamos a Aksha

		SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
		SDL_RenderFillRect(renderer, &pjA);

		//Barras de vida de feinryr y aksha

		DibujarBarraVida(renderer, feinryr, 20);
		DibujarBarraVida(renderer, aksha, 1700);

		SDL_RenderPresent(renderer);
		SDL_Delay(16); //Aproximadamente 60 FPS

	}
	SDL_DestroyTexture(fondo);
	SDL_DestroyTexture(sprite1);
	SDL_DestroyTexture(sprite2);
	SDL_DestroyTexture(sprite3);
	SDL_DestroyTexture(sprite4);
	SDL_DestroyTexture(sprite5);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);
	IMG_Quit();
	SDL_Quit();

return 0;
}
