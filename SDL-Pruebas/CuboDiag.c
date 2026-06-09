#include <SDL2/SDL.h>
#include <stdio.h>

int main()
{


	// 1. Inicializar SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
        	printf("Error inicializando SDL: %s\n", SDL_GetError());
        	return 1;
    	}

	// 2. Crear ventana
        SDL_Window *ventana = SDL_CreateWindow("Mover cuadrado",
					       SDL_WINDOWPOS_CENTERED,
					       SDL_WINDOWPOS_CENTERED,
					       640, 480,
					       SDL_WINDOW_SHOWN);

	if(!ventana)
	{
        	printf("Error creando ventana: %s\n", SDL_GetError());
        	SDL_Quit();
    		return 1;
    	}

	// 3. Crear renderizador
	SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

	// 4. Definir cuadrado inicial
    
	SDL_Rect cuadrado = {100, 100, 50, 50};

        int corriendo = 1;
    
	SDL_Event evento;

    
	// 5. Bucle principal
    
	while (corriendo)
        {
        
		// 5a. Capturar eventos (cerrar ventana)
        
		while (SDL_PollEvent(&evento))
        	{
                	if (evento.type == SDL_QUIT)
            		{
                		corriendo = 0;
            		}
        	}

		//Unit8; Es un entero sin signo, sirve para representar el estado de las teclas
		//Por lo que el puntero (*estado) apunta a un arreglo con todos los estados de teclas
		//
		//
		//Como SDL mantiene internamente un arreglo que se actualiza a tiempo real entonces:
		//SDL_GetKeyboardState(NULL); te devuelve un puntero que da acceso con este mismo arreglo interno guardandose en la variable/puntero "estado"
		//
		//
		const Uint8 *estado = SDL_GetKeyboardState(NULL);

		//Se usaran if independientes para medir si dos teclas son pulsadas al mismo tiempo

		if (estado[SDL_SCANCODE_UP])
        	{
        		cuadrado.y -= 5;
        	}
        	if (estado[SDL_SCANCODE_DOWN])
        	{
        		cuadrado.y += 5;
        	}
        	if (estado[SDL_SCANCODE_LEFT])
        	{
            		cuadrado.x -= 5;
        	}
	        if (estado[SDL_SCANCODE_RIGHT])
	        {
			cuadrado.x += 5;
	        }

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        	SDL_RenderClear(renderer);


		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        	SDL_RenderFillRect(renderer, &cuadrado);


		SDL_RenderPresent(renderer);
	        SDL_Delay(16);

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);
	SDL_Quit();
return 0;

}
