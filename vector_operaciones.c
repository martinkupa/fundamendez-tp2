#include <stdlib.h>
#include <assert.h>
#include "vector_operaciones.h"

void eliminar_objeto(objeto_t *objetos, 
                     int      *cantidad_objetos, 
                     int      indice_objeto)
{
    assert(objetos != NULL && "objetos no puede ser NULL");
    assert(cantidad_objetos != NULL && "cantidad_objetos no puede ser NULL");
    for (int i = indice_objeto; i < *cantidad_objetos-1; i++)
    {
        objetos[i] = objetos[i+1];
    }
    objetos[*cantidad_objetos-1] = (objeto_t){0};

    (*cantidad_objetos)--;
}

void eliminar_pedido(pedido_t *pedidos, 
                     int      *cantidad_pedidos, 
                     int      indice_objeto)
{
    assert(pedidos != NULL && "pedidos no puede ser NULL");
    assert(cantidad_pedidos != NULL && "cantidad_pedidos no puede ser NULL");
    for (int i = indice_objeto; i < *cantidad_pedidos-1; i++)
    {
        pedidos[i] = pedidos[i+1];
    }
    pedidos[*cantidad_pedidos-1] = (pedido_t){0};

    (*cantidad_pedidos)--;
}

void agregar_objeto(objeto_t *objetos, 
                    int      *cantidad_objetos, 
                    objeto_t nuevo_objeto)
{
    assert(objetos != NULL && "objetos no puede ser NULL");
    assert(cantidad_objetos != NULL && "cantidad_objetos no puede ser NULL");
    objetos[(*cantidad_objetos)++] = nuevo_objeto;
}

void agregar_pedido(pedido_t *pedidos, 
                    int      *cantidad_pedidos, 
                    pedido_t nuevo_pedido)
{
    assert(pedidos != NULL && "pedidos no puede ser NULL");
    assert(cantidad_pedidos != NULL && "cantidad_pedidos no puede ser NULL");
    pedidos[(*cantidad_pedidos)++] = nuevo_pedido;
}