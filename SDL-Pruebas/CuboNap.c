#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>



#define FILAS 30
#define COLUMNAS 60

int main()
{

        //SDL_Init(SDL_INIT_VIDEO); "SDL_Init" es quien inicia SDL, (SDL_INIT_VIDEO) es quien indica que se prepare el subsistema de video
        //es decir (ventanas, graficos, renderizadores)
        //
        //IMG_INIT(IMG_INIT_PNG); "IMG_Init" es quien activa los sopertes para ciertos formatos de imagen
        //IMG_INIT_PNG; Es quien indica que se quiere hacer soporte para archivos de tipo PNG
        //Para lineas mas adelante usar "IMG_Load", para que SDL_image sepa como leer este archivo
        //


        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);
        ///===
	///Se obtiene la resolucion de pantalla por defecto

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	int screenWidth = DM.w;
	int screenHeight = DM.h;
	///===
	//Tamaño de celdas para malla
	
	int BLOQUE_ANCHO = screenWidth / COLUMNAS;
	int BLOQUE_ALTO = screenHeight / FILAS;

	///
        SDL_Window *ventana = SDL_CreateWindow("Battle of reality",
                                               SDL_WINDOWPOS_CENTERED,
                                               SDL_WINDOWPOS_CENTERED,
                                               0, 0,
                                               SDL_WINDOW_FULLSCREEN_DESKTOP);

        SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
        ///===
        SDL_Surface *fondoSurface = IMG_Load("1MapaBattleOfReality.png");

        //Verificamos si la imagen se cargo correctamente
        if(!fondoSurface)
        {

                printf("Error cargando imagen: %s\n", IMG_GetError());
                return 1;

        }


        //SDL_CreateTextureFromSurface; convierte una "superficie" SDL_Surface en una "textura" SDL_Texture
        //Superficie: ES un bloque de memoria de la CPU que contiene pixeles de una imagen (cada pixel es guardado como dato en la RAM)
        //Textura:Termina por ser la version de esa imagen pero ya preparada para la GPU (Dibuja en la pantalla rapidamente)
        //(Vive en la memoria de video no en la RAM)
        //
        //SDL_FreeSurface(fondoSurface): Libera la memoria usada por la superficie original

        SDL_Texture *fondoTexture = SDL_CreateTextureFromSurface(renderer, fondoSurface);
        SDL_FreeSurface(fondoSurface);
	
	//=======
	//Cargamos el mapa desde el ,txt ya realizado
	

	int mapa[FILAS][COLUMNAS];

	FILE *archivo = fopen("1mapa30x60.txt", "r");

	if(!archivo)
	{
		printf("No se pudo abrir el 1mapa30x60.txt\n");
		return 1;
	}

	for(int i=0;i<FILAS;i++)
	{
		for(int j=0;j<COLUMNAS;j++)
		{
			fscanf(archivo, "%1d", &mapa[i][j]);
		}
	}

	fclose(archivo);

	//=======
	//Convertimos los bloques solidos invisibles
	
	SDL_Rect bloques[2000]; //30x60 = 1800 max, lo redondeo a 2k para evitar problemas :D
	int totalBloques=0; //cuantos bloques han sido creados

	for(int i=0;i<FILAS;i++)
	{
		for(int j=0;j<COLUMNAS;j++)
		{
			if(mapa[i][j]==1) //1=solido
			{
				bloques[totalBloques].x = j * BLOQUE_ANCHO; //horizontal
				bloques[totalBloques].y = i * BLOQUE_ALTO;  //vertical
				bloques[totalBloques].w = BLOQUE_ANCHO;     //ancho
				bloques[totalBloques].h = BLOQUE_ALTO;      //alto
				totalBloques++;

			}
		}
	}

        //=======
        //Ingresamos las dimensiones del cuadrado

        SDL_Rect cuadrado = {100, 100, 50, 50};
        int corriendo = 1;
        SDL_Event evento;

        //=======

        while(corriendo)
        {

                while(SDL_PollEvent(&evento))
                {
                        if(evento.type == SDL_QUIT)
                        {
                                corriendo=0;
                        }
                }

                //moviento del cubo en cuestion
                const Uint8 *estado = SDL_GetKeyboardState(NULL);

                SDL_Rect cubonuevo = cuadrado;

                if(estado[SDL_SCANCODE_UP])
                {
                        cubonuevo.y -=5;
                }
                if(estado[SDL_SCANCODE_DOWN])
                {
                        cubonuevo.y += 5;
                }
                if(estado[SDL_SCANCODE_LEFT])
                {
                        cubonuevo.x -= 5;
                }
                if(estado[SDL_SCANCODE_RIGHT])
                {
                        cubonuevo.x += 5;
                }
		
		//======
		//Colision con los bloques

		int colision=0;  // 0= no colision
		for(int i=0;i<totalBloques;i++) //recorre todos los bloques
		{
			if(SDL_HasIntersection(&cubonuevo, &bloques[i])) //verificacion de cubo
			{ 
			   	colision=1;  //colision=1
				break;
			}
		}

		if(!colision) //para que no haya bug
		{
			cuadrado=cubonuevo;
		}

		//======
                SDL_RenderCopy(renderer, fondoTexture, NULL, NULL);   //Nuestro fondo

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80);  //La malla en cuestion
								      
		for(int i=0;i<FILAS;i++)
		{
			for(int j=0;j<COLUMNAS;j++)
			{
				//Todas las celdas 
				SDL_Rect celda;
				celda.x = j* BLOQUE_ANCHO;
				celda.y = i* BLOQUE_ALTO;
				celda.w = BLOQUE_ANCHO;
				celda.h = BLOQUE_ALTO;

				SDL_RenderDrawRect(renderer, &celda);
			}
		}

		

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);     //Color de cubo
                SDL_RenderFillRect(renderer, &cuadrado);              //Muestra el cuadrado por ensima

                SDL_RenderPresent(renderer);                          //Muestra todo
		SDL_Delay(16);
        }

        SDL_DestroyTexture(fondoTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        IMG_Quit();
        SDL_Quit();



return 0;
}
