#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>

int main()
{

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	//Para las papu resoluciones a tiempo real
	
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	int screenWidth = DM.w;
	int screenHeigth = DM.h; 

	//Variables para el menu
	int corriendo = 1;
	int opcion = 0; //0=play ; 1=quit
	int timer= 0;

	//Efecto parpadeo
	int parpadeo = 0;

	//Creamos las ventanas
	SDL_Window *ventana = SDL_CreateWindow("Battle of Rality - Menu",
						SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED,
						0,0,
						SDL_WINDOW_FULLSCREEN_DESKTOP);
	
	SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

	//Ahora cargamos las papu imagenes
	
	SDL_Texture *MenuNormal = IMG_LoadTexture(renderer, "Menu0.png");    //Todo blanco
	SDL_Texture *MenuPlay = IMG_LoadTexture(renderer, "Menu1.png"); //Play amarillo
	SDL_Texture *MenuQuit = IMG_LoadTexture(renderer, "Menu2.png"); //Quit amarillo

	if(!MenuNormal || !MenuPlay || !MenuQuit)
	{
	
		printf("Error al cargar las imagenes: %s\n", IMG_GetError());
		return 1;

	}	

	//Iniciamos los evento para el menu
	
	SDL_Event evento;

	//
	
	while(corriendo)
	{
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				corriendo= 0;
			}

			if(evento.type == SDL_KEYDOWN)
			{	
				switch(evento.key.keysym.sym)
				{	
					case SDLK_UP:
					opcion = 0;
					break;

					case SDLK_DOWN:
					opcion = 1;
					break;

					case SDLK_RETURN:
					
					if(opcion == 0)
					{
						
						SDL_DestroyTexture(MenuNormal);
					        SDL_DestroyTexture(MenuPlay);
					        SDL_DestroyTexture(MenuQuit);
					        SDL_DestroyRenderer(renderer);
					        SDL_DestroyWindow(ventana);
					        IMG_Quit();
					        SDL_Quit();

						system("./2PjEnPantalla");  //Se ejecuta el juego (esto habra que cambiarlo varias veces)

					}
					else
					{
						printf("Quit a sido seleccionado\n");
						corriendo = 0;
					}
					break;
				}
			}
		}
	

		//Agregamos efecto de parpadeo + fondo
	
		timer++;
		parpadeo = (timer/20)%2;
	
		SDL_RenderClear(renderer);

		//Boton Play
	
		if(opcion == 0 && parpadeo)
		{
			SDL_RenderCopy(renderer, MenuPlay, NULL, NULL); //Play Amarillo							       
		}
		else if(opcion == 1 && parpadeo)
		{
			SDL_RenderCopy(renderer, MenuQuit, NULL, NULL); //Quit amarillo
		}
		else
		{
			SDL_RenderCopy(renderer, MenuNormal, NULL, NULL);
		}



		//Fps y Renderizado en cuestion
	
		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
	//Destrucciones
	
	SDL_DestroyTexture(MenuNormal);
	SDL_DestroyTexture(MenuPlay);
	SDL_DestroyTexture(MenuQuit);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);

	IMG_Quit();
	SDL_Quit();


return 0;
}
