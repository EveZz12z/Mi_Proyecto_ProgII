#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define FILAS 30
#define COLUMNAS 60

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	// ============================
	// OBTENER RESOLUCIÓN DE PANTALLA
 	// ============================
 	SDL_DisplayMode DM;
 	SDL_GetCurrentDisplayMode(0, &DM);

 	int screenWidth = DM.w;
 	int screenHeight = DM.h;

 	// Tamaño dinámico de cada celda
 	int BLOQUE_ANCHO = screenWidth / COLUMNAS;
 	int BLOQUE_ALTO  = screenHeight / FILAS;

 	// ============================
 	// CREAR VENTANA EN PANTALLA COMPLETA
 	// ============================
 	SDL_Window *ventana = SDL_CreateWindow("Battle of Reality",
       	                                       SDL_WINDOWPOS_CENTERED,
                                               SDL_WINDOWPOS_CENTERED,
                                               0, 0,
                                               SDL_WINDOW_FULLSCREEN_DESKTOP);

 	SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

 	// ============================
 	// CARGAR FONDO
 	// ============================
 	SDL_Surface *fondoSurface = IMG_Load("1MapaBattleOfReality.png");
 	if (!fondoSurface)
	{
        	printf("Error cargando imagen: %s\n", IMG_GetError());
        	return 1;
    	}

    	SDL_Texture *fondoTexture = SDL_CreateTextureFromSurface(renderer, fondoSurface);
    	SDL_FreeSurface(fondoSurface);

    	// ============================
    	// CARGAR MAPA DESDE TXT
    	// ============================
    	int mapa[FILAS][COLUMNAS];

    	FILE *archivo = fopen("1mapa30x60.txt", "r");
   	if (!archivo)
	{
        	printf("No se pudo abrir 1mapa30x60.txt\n");
        	return 1;
    	}

    	for (int i = 0; i < FILAS; i++)
	{
        	for (int j = 0; j < COLUMNAS; j++)
		{
            		fscanf(archivo, "%1d", &mapa[i][j]);
        	}
    	}
    	fclose(archivo);

    	// ============================
    	// CREAR BLOQUES SÓLIDOS INVISIBLES
    	// ============================
    	SDL_Rect bloques[2000]; // 30x60 = 1800 máx
    	int totalBloques = 0;

    	for (int i = 0; i < FILAS; i++)
	{
        	for (int j = 0; j < COLUMNAS; j++)
		{
            		if (mapa[i][j] == 1)
			{
                		bloques[totalBloques].x = j * BLOQUE_ANCHO;
                		bloques[totalBloques].y = i * BLOQUE_ALTO;
                		bloques[totalBloques].w = BLOQUE_ANCHO;
                		bloques[totalBloques].h = BLOQUE_ALTO;
                		totalBloques++;
            		}
        	}
    	}

    	// ============================
   	// CUBO
    	// ============================
    	SDL_Rect cuadrado = {100, 100, 50, 50};
    	int corriendo = 1;
    	SDL_Event evento;

    	//=============================
    	//Posicionamiento del cubo
    	//=============================

    	int filaInicio = 16;
    	int columnaInicio = 22;

    	cuadrado.x = columnaInicio * BLOQUE_ANCHO;
    	cuadrado.y = filaInicio * BLOQUE_ALTO - cuadrado.h;

    	//=====================================
	//Variables de para la gravedad + salto
	//=====================================
	
	float velocidadY = 0; //float pa que finquen los decimales y no sean tan bruscos los valores
	float gravedad = 0.5;
	int enElSuelo = 0;

	int saltos = 0;
	int maxSaltos = 2;
	int upPresionadoAntes = 0; //detectar lo del doble salto

	// ============================
    	// MOV DEL CUBO y +
    	// ============================
    	while (corriendo)
	{
		while (SDL_PollEvent(&evento))
		{
            		if (evento.type == SDL_QUIT)
			{
                		corriendo = 0;
            		}
        	}

        	const Uint8 *estado = SDL_GetKeyboardState(NULL);

        	SDL_Rect cubonuevo = cuadrado;

		//===================
		//Aplicacion de gravedad

		velocidadY += gravedad;      //La gravedad es 0.5 entonces por cada frame la velocidad aumenta en 0.5
		cubonuevo.y += velocidadY;   //La logica en toda su expresion "cubonuevo.y = (cubonuevo.y + velocidadY)"
					     //Si velocidadY es (+) el cubo bajara
					     //Si     ""     es (-) el cubo sube 
					     //Si     ""     es (0) el cubo no se mueve

	

		//====================
		//Movimiento Horizontal
		//====================

        	if (estado[SDL_SCANCODE_LEFT])
		{
			cubonuevo.x -= 5;
		}	
        	if (estado[SDL_SCANCODE_RIGHT])
		{
			cubonuevo.x += 5;
		}

		//=============================
		//Saltos
		//=============================
      
		int upAhora= estado[SDL_SCANCODE_UP];

		if(upAhora && !upPresionadoAntes && saltos < maxSaltos)
		{
			velocidadY = -10;
			saltos++;
			enElSuelo = 0;
		}

		upPresionadoAntes= upAhora;

		// ============================
        	// Colision Vertical (El suelo)
        	// ============================

		enElSuelo = 0;

        	for (int i = 0; i < totalBloques; i++) //se revisan todos los papu bloques solidos
		{
            		if (SDL_HasIntersection(&cubonuevo, &bloques[i])) //revisamos si el bloque choca con un bloque solido (gravedad en cuestion)
			{
                		if(velocidadY > 0) //Cuando el cubo vaya cayendo
				{
					cubonuevo.y = bloques[i].y - cuadrado.h;  //el cubo se quedara inmediatamente sobre el bloque solido
					enElSuelo = 1; //Es la magia de todo, permite salto, detener la gravedad, evita que siga acelerando hacia abajo
					saltos = 0; //reinicio de saltos al tocar el suelo
				}
				
				velocidadY = 0; //detiene la velocidad de caida
				break; //ya no revisa mas bloques
            		}
        	}

		//=====================
		//Colision Horizontal
		//=====================

		int colisionHorizontal = 0; //indica si hay una colision Horizontal 1=colision; 0=no colision
		SDL_Rect pruebaHorizontal = cuadrado; //Copia del cubo para verificar si hay algun tipo de mov ante una posible colision
		pruebaHorizontal.x = cubonuevo.x; //Esta sera la posicion donde quiera moverse el cubo en cuestion


		for(int i=0;i < totalBloques; i++)  //bucle que recorre todos los bloques solidosn del mapa
		{
			if(SDL_HasIntersection(&pruebaHorizontal, &bloques[i]))
			{
				colisionHorizontal=1;
				break;
			}
		}

		if(!colisionHorizontal) //en el caso de no haber colision de por medio movemos el papu cubo :D
		{
			cuadrado.x = cubonuevo.x;
		}

		//=============================
		//Actualizacion de posicion
		//=============================

		cuadrado.y = cubonuevo.y;

        	// ============================
        	// DIBUJAR FONDO
        	// ============================
        	SDL_RenderCopy(renderer, fondoTexture, NULL, NULL);

        	// ============================
        	// DIBUJAR MALLA (GRID)
        	// ============================
        	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80); // blanco suave

        	for (int i = 0; i < FILAS; i++)
		{
            		for (int j = 0; j < COLUMNAS; j++)
			{
                		SDL_Rect celda;
                		celda.x = j * BLOQUE_ANCHO;
                		celda.y = i * BLOQUE_ALTO;
                		celda.w = BLOQUE_ANCHO;
                		celda.h = BLOQUE_ALTO;

                	SDL_RenderDrawRect(renderer, &celda);
            		}
        	}
		
        	// ============================
        	// DIBUJAR CUBO
        	// ============================
        	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        	SDL_RenderFillRect(renderer, &cuadrado);

        	SDL_RenderPresent(renderer);
        	SDL_Delay(16);
    	}

    	SDL_DestroyTexture(fondoTexture);
    	SDL_DestroyRenderer(renderer);
    	SDL_DestroyWindow(ventana);
    	IMG_Quit();
    	SDL_Quit();

return 0;
}

