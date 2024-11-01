#ifndef __VECTOR_PEDIDOS_H__
#define __VECTOR_PEDIDOS_H__

#include <stdlib.h>
#include "restaurant.h"

typedef pedido_t *vector_pedidos_t;

/// @brief Reserva una instancia de vector_pedidos_t con capacidad para 
///        almacenar `cantidad_pedidos` elementos. Los elementos son 
///        inicializados a cero.
/// @param cantidad_pedidos La cantidad de elementos que podra almacenar el
///        vector.
/// @return Un puntero al vector reservado, o NULL en caso de fallo.
/// @pre cantidad_pedidos debe ser mayor a cero
vector_pedidos_t construir_vector_dinamico(int cantidad_pedidos);

/// @brief Agrega un nuevo pedido al tope del vector. En caso de fallo, `vector`
///        se deja intacto.
/// @note El buffer es reasignado en el proceso, en caso de exito `vector` sera
///       un puntero obsoleto.
/// @return Un puntero al nuevo vector_pedidos_t, o NULL en caso de fallo.
/// @pre vector no debe ser NULL
/// @pre cantidad_pedidos no debe ser NULL
vector_pedidos_t agregar_pedido_dinamico(vector_pedidos_t vector, 
                                         int              *cantidad_pedidos, 
                                         pedido_t         pedido_nuevo);

/// @brief Elimina el elemento en la posicion `indice_pedido`. En caso de fallo,
///        `vector` se deja intacto.
/// @note El buffer es reasignado en el proceso, en caso de exito `vector` sera
///       un puntero obsoleto.
/// @return Un puntero al nuevo vector_pedidos_t, o NULL en caso de fallo.
/// @pre vector no debe ser NULL
/// @pre cantidad_pedidos no debe ser NULL
vector_pedidos_t eliminar_pedido_dinamico(vector_pedidos_t vector, 
                                          int              *cantidad_pedidos, 
                                          int              indice_pedido);

/// @brief Destruye el `vector` y asigna `cantidad_pedidos` a cero.
/// @pre cantidad_pedidos no debe ser NULL
void eliminar_vector_dinamico(vector_pedidos_t vector,
                              int              *cantidad_pedidos);

#endif