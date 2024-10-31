#include "entrada.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


static bool caracter_en_array(unsigned int largo, 
                              const char   array[largo], 
                              char         busqueda);


char pedir_caracter_valido(unsigned int largo, 
                           const char   caracteres_validos[largo])
{
    assert(largo > 0 && "caracteres_validos no puede estar vacio");
    // de lo contrario el usuario estaria atascado en la pregunta por siempre
    assert(caracteres_validos != NULL && "caracteres_validos no puede ser NULL");
    char eleccion_usuario = '\0';
    bool eleccion_valida = false;
    do
    {
        // el espacio en el scanf evita que queden newlines en el buffer
        scanf(" %c", &eleccion_usuario); 
        eleccion_valida = caracter_en_array(largo, caracteres_validos, eleccion_usuario);
        if (!eleccion_valida)
            printf("Caracter invalido, intente nuevamente\n"); 
    } while (!eleccion_valida);
    return eleccion_usuario;
}

static bool caracter_en_array(unsigned int largo, 
                              const char   array[largo], 
                              char         busqueda)
{
    assert(array != NULL && "array no puede ser NULL");
    unsigned int i = 0;
    bool encontrado = false; 
    while (i < largo && !encontrado) 
    {
        encontrado = array[i] == busqueda;
        i++;
    }
    return encontrado; 
}
