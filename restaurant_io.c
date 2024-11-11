#include "restaurant_io.h"
#include "restaurant_utils.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/// @return true si `busqueda` este en `array`, de lo contrario false
static bool caracter_en_array(unsigned int largo, 
                              const char   array[largo], 
                              char         busqueda);

/// @brief Convierte filas y columnas del tablero a un indice lineal
/// @return (fila*MAX_COLUMNAS)+columna
/// @note En un principio habia implementado esto con type punning pero 
///       -fanalyzer se quejaba. Si C tuviera lambas esto seria una lambda en 
///       mostrar_juego, o si pudiera usar macros esto seria un #define
static size_t indice(coordenada_t coordenada);

/// @brief Dibuja las mesas en el buffer
/// @pre juego != NULL
/// @pre buffer != NULL
static void dibujar_mesas(const juego_t *juego, 
                          char          *buffer);
/// @brief Dibuja las herramientas en el buffer
/// @pre juego != NULL
/// @pre buffer != NULL
static void dibujar_herramientas(const juego_t *juego, 
                                 char          *buffer);

/// @brief Dibuja los obstaculos en el buffer
/// @pre juego != NULL
/// @pre buffer != NULL
static void dibujar_obstaculos(const juego_t *juego, 
                               char          *buffer);

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

void construir_string_terreno_juego(const juego_t *juego, 
                                    size_t        largo_buffer,
                                    char          buffer[largo_buffer])
{
    assert(juego != NULL && "juego no debe ser NULL");
    assert(buffer != NULL && "buffer no puede ser NULL");
    memset(buffer, '.', largo_buffer-1);
    buffer[largo_buffer-1] = '\0';

    for (int fila = 0; fila < MAX_FILAS; fila++)
        buffer[indice((coordenada_t){fila, MAX_COLUMNAS})] = '\n';
    
    dibujar_mesas(juego, buffer);

    buffer[indice((coordenada_t){juego->cocina.posicion.fil, juego->cocina.posicion.col})] = OBJ_COCINA;

    dibujar_herramientas(juego, buffer);

    dibujar_obstaculos(juego, buffer);

    coordenada_t posicion_mozo = juego->mozo.posicion; 
    buffer[indice((coordenada_t){posicion_mozo.fil, posicion_mozo.col})] = OBJ_LINGUINI;
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

static size_t indice(coordenada_t coordenada)
{
    return (size_t)((coordenada.fil*(MAX_COLUMNAS+1))+coordenada.col);
}

static void dibujar_mesas(const juego_t *juego, 
                          char          *buffer)
{
    assert(juego != NULL && "El juego no debe ser NULL");
    assert(buffer != NULL && "El buffer no debe ser NULL");
    for (int i = 0; i < juego->cantidad_mesas; i++)
    {
        const mesa_t *mesa = &juego->mesas[i];
        for (int j = 0; j < mesa->cantidad_lugares; j++)
        {
            bool hay_comensal = j < mesa->cantidad_comensales;
            char sprite = hay_comensal ? OBJ_COMENSAL : OBJ_MESA;
            buffer[indice((coordenada_t){mesa->posicion[j].fil, mesa->posicion[j].col})] = sprite;
        }
    }
}
static void dibujar_herramientas(const juego_t *juego, 
                                 char          *buffer)
{
    assert(juego != NULL && "El juego no debe ser NULL");
    assert(buffer != NULL && "El buffer no debe ser NULL");
    for (int i = 0; i < juego->cantidad_herramientas; i++)
    {
        const objeto_t *herramienta = &juego->herramientas[i];
        buffer[indice((coordenada_t){herramienta->posicion.fil, herramienta->posicion.col})] = herramienta->tipo;
    }
}
static void dibujar_obstaculos(const juego_t *juego, 
                               char          *buffer)
{
    assert(juego != NULL && "El juego no debe ser NULL");
    assert(buffer != NULL && "El buffer no debe ser NULL");
    for (int i = 0; i < juego->cantidad_obstaculos; i++)
    {
        const objeto_t *obstaculo = &juego->obstaculos[i];
        buffer[indice((coordenada_t){obstaculo->posicion.fil, obstaculo->posicion.col})] = obstaculo->tipo;
    }
}