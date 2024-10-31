#include "generador.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int generador_numero_aleatorio(int desde, 
                               int hasta) 
{
    assert(desde < hasta && "desde debe ser menor que hasta");
    return ( rand() % (hasta - desde) ) + desde;
}

coordenada_t generador_posicion_aleatoria(void) 
{
    return (coordenada_t){
        .fil = generador_numero_aleatorio(0, MAX_FILAS),
        .col = generador_numero_aleatorio(0, MAX_COLUMNAS)
    };
}

generador_t generador_constructor(void) 
{
    return (generador_t){0};
}

coordenada_t generador_posicion_unica(generador_t *generador)
{
    assert(generador != NULL && "generador no puede ser NULL");  
    coordenada_t pos_aleatoria;
    bool ocupado = false;
    do
    {
        pos_aleatoria = generador_posicion_aleatoria();
        ocupado = generador->posiciones_generadas_[pos_aleatoria.fil][pos_aleatoria.col];
    } while (ocupado);
    generador->posiciones_generadas_[pos_aleatoria.fil][pos_aleatoria.col] = true;
    return pos_aleatoria;
}

void generador_reiniciar_generador(generador_t *generador) 
{
    assert(generador != NULL && "generador no puede ser NULL");  
    memset(generador->posiciones_generadas_, 0, MAX_FILAS * MAX_COLUMNAS);    
}