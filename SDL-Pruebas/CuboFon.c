#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
	SDL_Window *ventana = SDL_CreateWindow("Fondo + cuadrado",
					       SDL_WINDOWPOS_CENTERED,
					       SDL_WINDOWPOS_CENTERED,
					       640, 480,
					       SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
	///===
	SDL_Surface *fondoSurface = IMG_Load("FondoBrawlhalla1.png");

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

		if(estado[SDL_SCANCODE_UP])
		{
			cuadrado.y -=5;
		}
		if(estado[SDL_SCANCODE_DOWN])
		{
			cuadrado.y += 5;
		}
		if(estado[SDL_SCANCODE_LEFT])
		{
			cuadrado.x -= 5;
		}
		if(estado[SDL_SCANCODE_RIGHT])
		{
			cuadrado.x += 5;
		}

		SDL_RenderCopy(renderer, fondoTexture, NULL, NULL);   //Nuestro fondo
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);     //Color de cubo
		SDL_RenderFillRect(renderer, &cuadrado);	      //Muestra el cuadrado por ensima
		SDL_RenderPresent(renderer);			      //Muestra todo


		SDL_Delay(16);
	}

	SDL_DestroyTexture(fondoTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);
	IMG_Quit();
	SDL_Quit();



return 0;
}
