/* 0: Sin posicionar, yendo hacia abajo
 * 1: Sin posicionar, yendo a la izquierda
 * 2: Sin posicionar, yendo hacia esquina izquierda superior
 * 3: Posicionado 1, ir hacia esquina derecha superior
 * 4: Posicionado 2, ir hacia esquina derecha inferior
 * 5: Posicionado 3, ir hacia izquierda
 * 6: Posicionado 4, ir hacia arriba
 * 7: Posicionado 5, ir hacia derecha
 * 8: Posicionado 6, ir hacia arriba */
int posicionado = 0;

int procesarIA(char** tab, int numFilas, int numColumnas, int filaActual, int columnaActual, int tecla_w, int tecla_a, int tecla_s, int tecla_d, int tecla_ia, int tecla_presionada)
{
	// Si se ha activado el modo IA, se considera que no se está posicionado
	if (tecla_presionada == tecla_ia) posicionado = 0;

	if (posicionado == 0)
	{
		// Ir hacia abajo
		if (filaActual == numFilas - 2)
		{
			// Si estamos en la penúltima fila, mover hacia abajo solo una vez más
			posicionado = 1;
		}
		return tecla_s;
	}
	else if (posicionado == 1)
	{
		// Ir a la izquierda del todo
		if (columnaActual == 1)
		{
			posicionado = 2;
		}
		return tecla_a;
	}
	else if (posicionado == 2)
	{
		// Ir hacia arriba del todo
		if (filaActual == 1)
		{
			posicionado = 3;
		}
		return tecla_w;
	}
	else if (posicionado == 3)
	{
		// Ir hacia esquina superior derecha
		if (columnaActual == numColumnas - 2)
		{
			posicionado = 4;
		}
		return tecla_d;
	}
	else if (posicionado == 4)
	{
		// Ir hacia esquina inferior derecha
		if (filaActual == numFilas - 2)
		{
			posicionado = 5;
		}
		return tecla_s;
	}
	else if (posicionado == 5)
	{
		// Ir hacia izquierda
		if (columnaActual == 1)
		{
			posicionado = 6;
		}
		return tecla_a;
	}
	else if (posicionado == 6)
	{
		// Ir hacia arriba para ir a la derecha
		if (filaActual == 1) posicionado = 3;
		else posicionado = 7;
		return tecla_w;
	}
	else if (posicionado == 7)
	{
		// Ir hacia derecha
		if (columnaActual == numColumnas - 3)
		{
			posicionado = 8;
		}
		return tecla_d;
	}
	else if (posicionado == 8)
	{
		// Ir hacia arriba para ir a la izquierda
		posicionado = 5;
		return tecla_w;
	}
	else return 0;
}
