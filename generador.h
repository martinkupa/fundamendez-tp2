#ifndef __GENERADOR_H__
#define __GENERADOR_H__
#include "restaurant.h"

/// @brief Una estructura para generar posiciones unicas NO MODIFICAR MIEMBROS
///        CON SUFIJO _, USE LAS FUNCIONES gen_*
typedef struct generador {
    bool posiciones_generadas_[MAX_FILAS][MAX_COLUMNAS];
} generador_t;

/// @brief genera un numero aleatorio en el rango [`desde`, `hasta`)
int generador_numero_aleatorio(int desde, 
                               int hasta);

/// @brief Genera una posicion aleatoria valida
coordenada_t generador_posicion_aleatoria(void);

generador_t generador_constructor(void);

/// @brief Genera posiciones aleatorias unicas, nunca devuelve dos veces la
///        misma posicion
/// @pre `generador` no puede ser NULL
/// @return Una posicion aleatoria unica
coordenada_t generador_posicion_unica(generador_t *generador);
    
/// @brief Reinicia el historial de posiciones generadas
/// @pre generador no puede ser NULL
void generador_reiniciar_generador(generador_t *generador);
#endif