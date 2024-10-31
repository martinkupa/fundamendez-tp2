#ifndef __ENTRADA_H__
#define __ENTRADA_H__

/// @brief le pide al usuario un caracter valido
/// @param largo el largo de `caracteres_validos`
/// @param caracteres_validos un array con los caracteres considerados validos,
///        debe tener al menos un elemento
/// @return el caracter elegido por el usuarios
/// @pre array != NULL
/// @pre largo > 0
char pedir_caracter_valido(unsigned int largo, 
                           const char   caracteres_validos[largo]);


#endif