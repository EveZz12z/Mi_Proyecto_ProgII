

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>

#define FILAS 30
#define COLUMNAS 60

//Bloque solido (sprite)

typedef struct
{

	SDL_Rect rect; //para la posicion y tamaño del bloque
	int tipo;	//Tipo de sprite
} Bloque;

int main()
{
	
	//Iniciamos SDL en cuestion
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	//matriz de mapa
	
	int mapa[FILAS][COLUMNAS];
	int i=0,j=0;

	//Variable para crear bloques solidos
	
	int TotalBloques = 0; //contador que aumenta segun los tipos de bloques solidos que agregue en el mapa 
	int valor; 	      //Variable que sirve para ver si los digitos/valores ingresados en el txt seran bloques solidos o no
	int ColisionH = 0;    //Variable para las colisiones horizontales 0=no hay / 1= si que hay
 
	//Variables para la gravedad y +

	float VelocidadY=0; //Es quien medira la velocidad al caer del pj x la gravedad
	float Gravedad=0.5; 
	int EnElSuelo=0;    //indicador que dira si el jugador estara en el suelo o en el aire 0=en el aire / 1=suelo/plataforma

	int Saltos=0;       //Cuantos saltos a hecho el pj
	int MaxSaltos=2;    //La cantidad max de saltos permitidos
	int SaltoAntes=0;   //Para evitar que mantener la tecla pulsada se active muchas veces
	int SaltoAhora=0;

	int corriendo=1;    //variable que controla si el juego se mantiene o se cierra 1= se mantiene 0=se cierra (termina)

	//Resolucion de pantalla
	
	int screenWidth = 1920;
	int screenHeight = 1080;

	int BLOQUE_ANCHO = screenWidth / COLUMNAS; // 1920/60=32
	int BLOQUE_ALTO = screenHeight / FILAS;    // 1080/30=36
						   //Por ende mis cuadrados seran de 32x36 pixeles

	//Ventana
	SDL_Window *ventana = SDL_CreateWindow("Battle of Reality",
						SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED,
						screenWidth,
						screenHeight,
						0);

	SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

	//Cargamos nuestro fondo
	
	SDL_Texture *fondo = IMG_LoadTexture(renderer, "1MapaBattleOfReality.png");
	
	//Cargamos los sprites
	
	SDL_Texture *sprite1 = IMG_LoadTexture(renderer, "Sprite4-Bloque1.png");
	SDL_Texture *sprite2 = IMG_LoadTexture(renderer, "Sprite4-Bloque2.png");
	SDL_Texture *sprite3 = IMG_LoadTexture(renderer, "Sprite4-Bloque3.png");
	SDL_Texture *sprite4 = IMG_LoadTexture(renderer, "Sprite4-Bloque4.png");
	SDL_Texture *sprite5 = IMG_LoadTexture(renderer, "Sprite5-Bloque1.png");
	

	//Cargamos el mapa
	
	FILE *archivo = fopen("1mapa30x60.txt", "r");  //abrimos puntero *archivo para leer y recorrer el txt

	for(i=0;i<FILAS;i++)
	{
		for(j=0;j<COLUMNAS;j++)	
		{
			fscanf(archivo,"%1d", &mapa[i][j]);  //Lee los digitos y los guarda en mapa[i][j]
		}
	}

	fclose(archivo);  //lo cerramos junto a *archivo para evitar errores y liberar memoria

	//Creacion de bloques solidos 
	
	Bloque bloques[2000];

	for(i=0;i<FILAS;i++)
	{
		for(j=0;j<COLUMNAS;j++)
		{
			
			valor = mapa[i][j];

			if(valor >= 1 && valor <=5)
			{
				bloques[TotalBloques].rect.x = j * BLOQUE_ANCHO;  //columna (j) x tamaño de pixeles (BLOQUE_ANCHO) = posicion en base a pixeles 
				bloques[TotalBloques].rect.y = i * BLOQUE_ALTO;   //fila (i)  x  tamaño de pixeles (BLOQUE_ALTO)  = posicion vertical en pixeles
				bloques[TotalBloques].rect.w = BLOQUE_ANCHO;      //definimos el ancho de los bloque
				bloques[TotalBloques].rect.h = BLOQUE_ALTO;       //definimos el alto de los bloques

				bloques[TotalBloques].tipo = valor;

				TotalBloques++;
			}
		}
	}

	//Pj temporal (solo es un cubo morado lol)
	
	SDL_Rect pj;

	pj.w= 50; //ancho en pixeles
	pj.h= 50; //altura en pixeles

	pj.x= 22 * BLOQUE_ANCHO;       //ver esto al casi terminar
	pj.y= 16 * BLOQUE_ALTO - pj.h; //
	

	SDL_Event evento;

	//EL juego en cuestion
	
	while(corriendo)
	{
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				corriendo=0;
			}
		}
			
		const Uint8 *estado = SDL_GetKeyboardState(NULL);  //detectamos si las teclas estan pulsadas o no y actualizamos al Rect pj con un nuevo Rect
						
		SDL_Rect nuevo = pj;

		//gravedad en cuestion
	
		VelocidadY += Gravedad; //Se le sumara 0.5 (Gravedad) x frame a VelocidadY, por lo cual mientras mayor sea el numero de VelocidadY, mas rapido caera pj

		nuevo.y += VelocidadY;	//Evitamos errores de atravesar bloques
			
		//Mov horizontal

		if(estado[SDL_SCANCODE_LEFT])
		{
			nuevo.x -=5;
		}

		if(estado[SDL_SCANCODE_RIGHT])
		{
			nuevo.x += 5;	
		}


		//Saltos

		SaltoAhora = estado[SDL_SCANCODE_UP];   //Guarda si la tecla de salto esta presionada en ese frame 1=si 0=no

		if(SaltoAhora && !SaltoAntes && Saltos < MaxSaltos) //la tecla de salto debe ser presionada / verifica que la tecla no estaba pulsada en el frame anterior /
								    //Se controla cuantos saltos se podra llegar a hacer
				
		{		
			VelocidadY= -10; //Valor negativo para que el pj pueda subir/saltar
			Saltos++;	 //Contador de saltos
			EnElSuelo= 0;    //Revisa si el pj esta en el aire para evitar que se piense que sigue en el suelo

		}


		SaltoAntes = SaltoAhora; //Actualiza el estado de la tecla para los siguientes frames

		//Colisiones en cuestion

		EnElSuelo = 0; //Es quien detectara si existe una colision de por medio 0 = no existe colision /  1 = existe colision de por medio

		for(i=0;i<TotalBloques;i++)	//recorremos todos los bloques solidos del mapa
		{
			if(SDL_HasIntersection(&nuevo, &bloques[i].rect)) //revisamos si nuestro pj "rect nuevo" choca con el bloque actual
			{
				if(VelocidadY > 0) //se detecta si el pj esta cayendo 
				{
					nuevo.y = bloques[i].rect.y - pj.h;  //dejara al pj justo sobre el bloque solido sin atravesar este mismo.
					EnElSuelo = 1; //marca que el pj esta en el suelo 1=solido/suelo 0=aire/salto
					Saltos = 0; //reiniciamos contador de saltos para que el pj pueda saltar otra vez
				}

				VelocidadY = 0;  //detenemos y reiniciamos la gravedad en cuestion
				break;
			}
		}
			
		//Ahora para las Horizontales (colision)
		
		ColisionH=0; 
		SDL_Rect prueba = pj; //creamos un rect temporal para prueba, este es una copia del pj que simula el mov horizontal
		prueba.x = nuevo.x;   //cambia la posicion del rect de prueba

		for(i=0;i<TotalBloques;i++) //recorre todos los bloques solidos del mapa en cuestion
		{
			if(SDL_HasIntersection(&prueba, &bloques[i].rect)) //si hay colision = 1 / no hay colision = 0; / si hay colision marca ColisionH=1
			{
				ColisionH=1;
				break;
			}
		}

		if(!ColisionH) //en el caso de no haber colision
		{
			pj.x = nuevo.x; //Actualiza la posicion del pj 
		}

		pj.y = nuevo.y; //actualiza verticalmente el pj (esto siempre se hara pq la colision ya fue calculada antes por el rect de prueba)
			

		//Comenzamos a dibujar todo
			
		SDL_RenderCopy(renderer, fondo, NULL, NULL);

		//Dibujamos las plataformas
			
		for(i=0;i<TotalBloques;i++)
		{
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

		//Dibujamos el pj temporal (el cubo)

		SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
		SDL_RenderFillRect(renderer, &pj);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);


		
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);
	IMG_Quit();
	SDL_Quit();

return 0;
}
