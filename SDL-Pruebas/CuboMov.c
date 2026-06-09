#include<stdio.h>
#include<SDL2/SDL.h>
//Esta libreria SDL es la que logra incluir las funciones SDL (Ventanas, graficos, eventos)

//gcc gcc [nombre.c] -o [nombre] -lSDL2 -lSDL2_image
//./[Nombre]

int main()
{
//SDL_Init; es la que inicia SDL (Pone en marcha el sistema interno)
//Por otro lado SDL_INIT_VIDEO; Se le indica a SDL que queremos usar parte de sus graficos

	if(SDL_Init(SDL_INIT_VIDEO)!=0)
	{
		//SDL_GetError; es quien le indica al usuario que existe un error al intentar iniciar SDL
		printf("Error inicializado SDL: %s\n", SDL_GetError());
		return 1;
	}
	//SDL_Window Es el que declara un puntero a una ventana
	//
	//SDL_CreateWindow es quien crea una ventana
	//
	//"Mover cuadrado" es el texto que aparecera en la parte superior
	//
	//SDL_WINDOW_CENTERED, SDL_WINDOWPOS_CENTERED; Son quienes ven la posicion en este caso quedara centrado de manera vertical y horizontal
	//
	//640, 480; Son los que dan el tamño a la ventan en pixeles (Ancho x Alto)
	//
	//SDL_WINDOW_SHOW; Una bandera que le indicara a la ventana que se muestre inmediatamente
	//Ademas de SDL_WINDOW_SHOW existes; SDL_WINDOW_FULLSCREEN - SDL_WINDOW_RESIZABLE
	//FULLSCREEN; Pantalla completa
	//RESIZABLE; Redimensionable (ver mas a detalle)

	SDL_Window *ventana = SDL_CreateWindow("Mover cuadrado",
					        SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED,
						640, 480,
						SDL_WINDOW_SHOWN
					      );


	// =========================
	
        //SDL_CreateRenderer; Crea el pincel que permite dibujar dentro de la ventana (como un tipo de arreglo)	
	//SDL_RENDERER_ACCELERATED; es quien dice que se use la GPU en vez de la CPU para que sea mas rapido
	SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);


	//SDL_Rect; Estructura que define un rectangulo
	//{x, y, ancho, alto} (Posicion)
	SDL_Rect cuadrado = {50, 50, 100, 100};


	//corriendo; Bandera que vera si el juego sigue activo
	//SDL_Event; Variable que guarda los eventos
	//Eventos; Pulsacion de tecla, Soltar una tecla, Movimiento de Mouse, Click
	int corriendo = 1;
	SDL_Event evento;

	//=========

	//While(corriendo); El bucle principal del programa, mientras corriendo = 1 el juego seguira en funcionamiento
	//
	//While(SDL_PollEvent(&evento))
	//Este bucle es quien revisa si hay algun evento pendiente, cada vez que se detecte un evento este se guarda
	//
	//if(evento.type == SDL_QUIT)
	//Si el evento es cerrar ventana lleva a cabo la siguiente accion "corriendo = 0;" por lo que termina el programa
	//
	//if(evento.type == SDL_KEYDOWN)
	//Si en el evento una tecla fue pulsada entonces se llevada hasta el switch que determinara que tecla fue
	//(Tener en cuenta que SDL_KEYDOWN Solo tomara un evento es decir que no tomara un evento multiple por asi decirlo)
	//Para hacer que el cubo se mueva hacia una diagonal ver el "CuboDiag.c"
	//
	//switch(evento.key.keysym.sym)
	//Este seria el switch que dira que tecla fue pulsada (LAS FLECHAS)
	

	while(corriendo)
	{
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				corriendo = 0;
			}

			if(evento.type == SDL_KEYDOWN)
			{
				switch(evento.key.keysym.sym)
				{
				
					case SDLK_UP:
					     cuadrado.y -=10;
				     	     break;

					case SDLK_DOWN:
					     cuadrado.y +=10;
					     break;

					case SDLK_LEFT:
					     cuadrado.x -=10;
					     break;

					case SDLK_RIGHT:
					     cuadrado.x +=10;
					     break;	     

				}
			}
		}	
	

	//=============
	
	//SDL_SetRenderDrawColor; define el color del dibujo
	//SDL_RenderClear; Limpia la pantalla de este color


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//=========
	

	//Volvemos a definir el color

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &cuadrado);

	//========
	
	//Muestra por pantalla todo lo que se dibujo en el ciclo
	SDL_RenderPresent(renderer);

	//=======
	
	
	//Pausa de 16 milisegundos (app: 60fps)
	//Esto evita que se consuma toda la CPU

	SDL_Delay(16);

	}
	//======
	
	//SDL_DestroyRenderer / SDL_DestroyWindow; son quienes liberan la memoria
	//SDL_Quit; Es quien finalmente termina el programa de manera correcta

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);
	SDL_Quit();



return 0;
}
