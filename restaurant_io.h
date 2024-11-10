#ifndef __RESTAURANT_IO_H__
#define __RESTAURANT_IO_H__

#include "restaurant.h"

#include <stdlib.h>

/// @brief le pide al usuario un caracter valido
/// @param largo el largo de `caracteres_validos`
/// @param caracteres_validos un array con los caracteres considerados validos,
///        debe tener al menos un elemento
/// @return el caracter elegido por el usuarios
/// @pre array != NULL
/// @pre largo > 0
char pedir_caracter_valido(unsigned int largo, 
                           const char   caracteres_validos[largo]);

/// @brief Plasma el estado actual del terreno de juego en `buffer`
/// @pre juego no puede ser NULL
/// @pre buffer no puede ser NULL
void construir_string_terreno_juego(const juego_t *juego, 
                                    size_t        largo_buffer,
                                    char          buffer[largo_buffer]);
#endif