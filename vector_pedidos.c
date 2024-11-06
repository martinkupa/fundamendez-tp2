#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "vector_pedidos.h"
#include "vector_operaciones.h"



vector_pedidos_t construir_vector_dinamico(int cantidad_pedidos)
{
    assert(cantidad_pedidos > 0 && "cantidad_pedidos debe ser mayor a 0");

    vector_pedidos_t buffer_pedidos = calloc((size_t)cantidad_pedidos, sizeof(pedido_t));

    return buffer_pedidos;
}

vector_pedidos_t agregar_pedido_dinamico(vector_pedidos_t vector, 
                                         int              *cantidad_pedidos, 
                                         pedido_t         pedido_nuevo)
{
    assert(cantidad_pedidos != NULL && "cantidad_pedidos no puede ser NULL");
    
    size_t capacidad_requerida = (size_t)(*cantidad_pedidos+1)*sizeof(pedido_t);
    vector_pedidos_t buffer_extendido = realloc(vector, capacidad_requerida);

    if (!buffer_extendido)
        return NULL;
    
    vector = NULL;

    agregar_pedido(buffer_extendido, cantidad_pedidos, pedido_nuevo);

    return buffer_extendido;
}

vector_pedidos_t eliminar_pedido_dinamico(vector_pedidos_t vector, 
                                          int              *cantidad_pedidos, 
                                          int              indice_pedido)
{
    assert(vector != NULL && "vector no puede ser NULL");
    assert(cantidad_pedidos != NULL && "cantidad_pedidos no puede ser NULL");
    assert(indice_pedido >= 0 && indice_pedido < *cantidad_pedidos && "indice_pedido debe estar en rango");
    
    pedido_t copia_temporal = vector[*cantidad_pedidos-1];

    size_t capacidad_requerida = (size_t)(*cantidad_pedidos-1)*sizeof(pedido_t);

    vector_pedidos_t buffer_acortado = realloc(vector, capacidad_requerida);

    if (!buffer_acortado)
    {
        bool vector_liberado = errno != ENOMEM;
        if (vector_liberado)
            *cantidad_pedidos = 0;    
        return NULL;
    }

    vector = NULL;

    (*cantidad_pedidos)--;

    if (indice_pedido == *cantidad_pedidos)
        return buffer_acortado;

    // no puedo llamar a eliminar_pedido por que decrementaria cantidad_pedidos dos veces
    for (int i = indice_pedido; i < *cantidad_pedidos-1; i++)
        buffer_acortado[i] = buffer_acortado[i+1];

    buffer_acortado[*cantidad_pedidos-1] = copia_temporal;

    return buffer_acortado;
}


void eliminar_vector_dinamico(vector_pedidos_t vector,
                              int              *cantidad_pedidos)
{
    assert(cantidad_pedidos != NULL && "cantidad_pedidos no puede ser NULL");
    free(vector);
    *cantidad_pedidos = 0;
}