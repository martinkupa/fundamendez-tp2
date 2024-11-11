#ifndef __VECTOR_PEDIDOS_H__
#define __VECTOR_PEDIDOS_H__

#include <stdlib.h>
#include "restaurant.h"

typedef pedido_t *vector_pedidos_t;

/// @brief Agrega un nuevo pedido al tope del vector. En caso de fallo, `vector`
///        se deja intacto. Si `vector` es NULL, reserva un nuevo 
///        vector_pedidos_t
/// @note El buffer es reasignado en el proceso, en caso de exito `vector` sera
///       un puntero obsoleto.
/// @return Un puntero al nuevo vector_pedidos_t, o NULL en caso de fallo.
/// @pre cantidad_pedidos no debe ser NULL
vector_pedidos_t agregar_pedido_dinamico(vector_pedidos_t vector, 
                                         int              *cantidad_pedidos, 
                                         pedido_t         pedido_nuevo);

/// @brief Elimina el elemento en la posicion `indice_pedido`. En caso de fallo
///        al reservar, `vector` se deja intacto y ENOMEM se guarda en errno. 
///        En caso de eliminar el ultimo elemento, se libera el vector.
/// @note El buffer es reasignado en el proceso, en caso de exito `vector` sera
///       un puntero obsoleto.
/// @return Un puntero al nuevo vector_pedidos_t, NULL en caso de fallo o al
///         liberar el vector. La manera de diferenciar estos casos es mediante
///         el valor de `cantidad_pedidos` (0 si fue liberado) o errno.
/// @pre vector no debe ser NULL
/// @pre cantidad_pedidos no debe ser NULL
/// @pre indice_pedido debe estar en rango
vector_pedidos_t eliminar_pedido_dinamico(vector_pedidos_t vector, 
                                          int              *cantidad_pedidos, 
                                          int              indice_pedido);

/// @brief Destruye el `vector` y asigna `cantidad_pedidos` a cero.
/// @pre cantidad_pedidos no debe ser NULL
void eliminar_vector_dinamico(vector_pedidos_t vector,
                              int              *cantidad_pedidos);

#endif