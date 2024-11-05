#include "restaurant.h"
#include "restaurant_io.h"
#include "restaurant_utils.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    srand((unsigned)time(NULL));
    juego_t juego;
    inicializar_juego(&juego);

    bool juego_terminado;
    do
    {
        system("clear");
        mostrar_juego(juego);
        printf("Ingrese una accion (%c,%c,%c,%c,%c,%c): ", ACCION_ARRIBA, ACCION_IZQUIERDA, ACCION_ABAJO, ACCION_DERECHA, ACCION_MOPA, ACCION_PATIN);
        // Es necesario flushear el stream manualmente cuando printf no recibe un newline
        fflush(stdout);
        char accion = pedir_caracter_valido(CANTIDAD_ACCIONES, (char[]){ACCION_ARRIBA, ACCION_IZQUIERDA, ACCION_ABAJO, ACCION_DERECHA, ACCION_MOPA, ACCION_PATIN});
        realizar_jugada(&juego, accion);
        juego_terminado = estado_juego(juego) != CONTINUA; 
    } while (!juego_terminado);

    bool juego_ganado = estado_juego(juego) == GANO;
    printf(juego_ganado?"Ganaste\n":"Perdiste\n"); 
    return 0;
}