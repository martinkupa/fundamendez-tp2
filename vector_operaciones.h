#ifndef __VECTOR_OPERACIONES_H__
#define __VECTOR_OPERACIONES_H__

#include "restaurant.h"

/// @brief Agrega `nuevo_objeto` al tope del vector de objetos
/// @pre objetos no puede ser NULL
/// @pre cantidad_objetos no puede ser NULL
void agregar_objeto(objeto_t *objetos, int *cantidad_objetos, objeto_t nuevo_objeto);
/// @brief Agrega `nuevo_pedido` al tope del vector de pedidos
/// @pre pedidos no puede ser NULL
/// @pre cantidad_pedidos no puede ser NULL
void agregar_pedido(pedido_t *pedidos, int *cantidad_pedidos, pedido_t nuevo_pedido);

/// @brief Elimina el elemento en la posicion `indice_objeto`. Como consecuencia
///        el ultimo elemento es asignado a cero.
/// @pre objetos no puede ser NULL
/// @pre cantidad_objetos no puede ser NULL
void eliminar_objeto(objeto_t *objetos, int *cantidad_objetos, int indice_objeto);

/// @brief Elimina el elemento en la posicion `indice_pedido`. Como consecuencia
///        el ultimo elemento es asignado a cero.
/// @pre pedidos no puede ser NULL
/// @pre cantidad_pedidos no puede ser NULL
void eliminar_pedido(pedido_t *pedidos, int *cantidad_pedidos, int indice_pedido);

#endif