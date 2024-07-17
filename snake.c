#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include "inteligenciaSnake.h"

#define NUM_FILAS 12
#define NUM_COLUMNAS 30
#define TIEMPO_ESPERA 100 // En milisegundos

#define AIRE ' '
#define CUERPO 'o'
#define CABEZA 'O'
#define CHOQUE 'X'
#define COMIDA '@'

#define TECLA_W 119 
#define TECLA_A 97
#define TECLA_S 115
#define TECLA_D 100
#define TECLA_IA 120

struct serp
{
	int tiempoVida;
	int fila;
	int columna;
};

void colocarComida(char** mat)
{
    int filaRand, columnaRand;
    int colocado = 0;
	while (colocado == 0)
	{
		filaRand = rand() % NUM_FILAS;
		columnaRand = rand() % NUM_COLUMNAS;

		if (mat[filaRand][columnaRand] == AIRE)
		{
		    mat[filaRand][columnaRand] = COMIDA;
			colocado = 1;
		}
	}
}

void rellenarTablero(char** mat)
{
	int i, j;
	for (i = 0; i < NUM_FILAS; i++)
	{
		for (j = 0; j < NUM_COLUMNAS; j++)
		{
			mat[i][j] = AIRE;
		}
	}
}

void imprimirTablero(char** mat, int score)
{
	curs_set(0);
	int i, j;
    	
	move(0, 0);
	printw("Puntuación: %d", score);
	move(1, 0);
	printw("%c", 'O');
	for (i = 0; i < NUM_COLUMNAS; i++)
	{
		printw("%c", '=');
	}
	printw("%c", 'O');
	
	move(2, 0);
	for (i = 0; i < NUM_FILAS; i++)
	{
		printw("%c", '|');
		for (j = 0; j < NUM_COLUMNAS; j++)
		{
			printw("%c", mat[i][j]);
		}
		printw("%c", '|');
		move(3 + i, 0);
	}

	printw("%c", 'O');
	for (i = 0; i < NUM_COLUMNAS; i++)
	{
		printw("%c", '=');
	}
	printw("%c", 'O');

    refresh();
}

int empezarPartida(char** tab, struct serp* cuerpo)
{
	int tam = 3;
	int terminada = 0;
	int filaActual = cuerpo[0].fila;
	int columnaActual = cuerpo[0].columna;
	int tecla = TECLA_D;
	int presionado = -1;
	int posLibre = 0;
	int alargar = 0;
	int score = 0;
	int modoIA = 0;

	while (terminada == 0)
	{
		// Se actualiza el tiempo de vida de todo el cuerpo
		if (alargar == 0)
		{
				for (int i = 0; i < tam; i++)
				{
				  cuerpo[i].tiempoVida--;
				  if (cuerpo[i].tiempoVida == 0)
				  {
								// Se borra la última parte del cuerpo (la que tiene 0 de vida)
								// Y guardamos la posicion del cuerpo que ahora esta libre
								tab[cuerpo[i].fila][cuerpo[i].columna] = AIRE;
								posLibre = i;
				  }
				}
		}
		
		// La cabeza ya no es la cabeza, es una parte del cuerpo
		tab[filaActual][columnaActual] = CUERPO;

		presionado = getch();
		if (presionado == TECLA_W || presionado == TECLA_A ||
			presionado == TECLA_S || presionado == TECLA_D)
		{
			// No cambiar de direccion hacia atras
			if (!((tecla == TECLA_W && presionado == TECLA_S) ||
				(tecla == TECLA_A && presionado == TECLA_D) ||
				(tecla == TECLA_S && presionado == TECLA_W) ||
				(tecla == TECLA_D && presionado == TECLA_A)))
			{
				tecla = presionado;
			}
		}
		// Cambiar entre el control manual o por IA
		else if (presionado == TECLA_IA)
		{
			if (modoIA == 0) modoIA = 1;
			else modoIA = 0;
		}

		if (modoIA == 1) tecla = procesarIA(tab, NUM_FILAS, NUM_COLUMNAS, filaActual, columnaActual, TECLA_W, TECLA_A, TECLA_S, TECLA_D, TECLA_IA, presionado);
		
		if (tecla == TECLA_W) filaActual--;
		else if (tecla == TECLA_A) columnaActual--;
		else if (tecla == TECLA_S) filaActual++;
		else if (tecla == TECLA_D) columnaActual++;
		else 
		{
			perror("Error: Movimiento no declarado");
			exit(1);
		}
		

		// Si en la posición a moverse no es AIRE entonces se
		// coloca CHOQUE y se termina la partida.
		// Si es aire, se coloca una nueva cabeza y la anterior pasa a
		// tener sprite de cuerpo. 
		if (filaActual >= NUM_FILAS || columnaActual >= NUM_COLUMNAS ||
			filaActual < 0 || columnaActual < 0) 
		{
			terminada = 1;
		}
		else if (tab[filaActual][columnaActual] == CUERPO)
		{
			tab[filaActual][columnaActual] = CHOQUE;
			terminada = 1;
			imprimirTablero(tab, score);	
		}
		else
		{
      if (alargar == 0)
			{
				cuerpo[posLibre].tiempoVida = tam;
				cuerpo[posLibre].fila = filaActual;
				cuerpo[posLibre].columna = columnaActual;
			}
      else
			{
				cuerpo[tam].tiempoVida = tam + 1;
				cuerpo[tam].fila = filaActual;
				cuerpo[tam].columna = columnaActual;
				alargar = 0;
				tam++;
				score++;
			}

			if (tab[filaActual][columnaActual] == COMIDA) alargar = 1;
			tab[filaActual][columnaActual] = CABEZA;

			if (alargar == 1) colocarComida(tab);
      imprimirTablero(tab, score);

	  // Si se ganó la partida...
	  if (tam == NUM_FILAS * NUM_COLUMNAS - 1) terminada = 2;
      usleep(TIEMPO_ESPERA * 1000); // Tick
    }
	}
	return terminada;
}

int main()
{
	srand(time(NULL));
	// Empezar impresion de tablero especial
	initscr();
	timeout(1);
	cbreak();
	noecho();

	if (NUM_FILAS < 2 || NUM_COLUMNAS < 4)
	{
		printf("El numero de filas y columnas es muy pequeño\n");
		return 1;
	}

	// Crear tablero
	char** tablero = (char**)malloc(NUM_FILAS * sizeof(char*));
	for (int i = 0; i < NUM_COLUMNAS; i++)
	{
		tablero[i] = (char*)malloc(NUM_COLUMNAS * sizeof(char));
	}
	rellenarTablero(tablero);

	// Crear serpiente
	struct serp* cuerpo = (struct serp*)malloc(NUM_FILAS*NUM_COLUMNAS*sizeof(struct serp));
	for (int i = 0; i < 3; i++)
	{
		cuerpo[i].tiempoVida = 3 - i;
		cuerpo[i].fila = (NUM_FILAS / 2) - 1;
		cuerpo[i].columna = (NUM_COLUMNAS / 2) - i;

		if (i == 0) tablero[cuerpo[i].fila][cuerpo[i].columna] = CABEZA;
		else tablero[cuerpo[i].fila][cuerpo[i].columna] = CUERPO;
	}
	
    colocarComida(tablero);
	imprimirTablero(tablero, 0);
	usleep(TIEMPO_ESPERA * 1000);
	int res = empezarPartida(tablero, cuerpo);
	move(0, 16);
	if (res == 1) printw("| Has perdido");
	else printw("| HAS GANADO!!!");
	refresh();
	usleep(2000000);
	endwin();

	// Liberar tablero
	for (int i = 0; i < NUM_FILAS; i++)
	{
		free(tablero[i]);
	}
	free(tablero);
	free(cuerpo);
	return 0;
}
