#include "restaurant_utils.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>


// Definicion de constantes.

const int GANO = 1;
const int CONTINUA = 0;
const int PERDIO = -1;
const int32_t OBJETIVO_DINERO = 150000;
const uint8_t LIMITE_MOVIMIENTOS = 200;
const tipo_mesas_t MESAS_1X1 = 1;
const tipo_mesas_t MESAS_2X2 = 2;
const uint16_t PAGO_MESA_1X1 = 5000;
const uint16_t PAGO_MESA_2X2 = PAGO_MESA_1X1 * 4;
const int8_t CANTIDAD_MESAS_1X1 = 6;
const int8_t CANTIDAD_MESAS_2X2 = 4;
const uint8_t CANTIDAD_CHARCOS = 5;
const uint8_t CANTIDAD_PATINES = 5;
const uint8_t CANTIDAD_MONEDAS = 8;
const uint8_t INDICE_MOPA = 0;
const uint8_t PACIENCIA_MINIMA = 100;
const uint8_t PACIENCIA_MAXIMA = 200;
const int NO_SUPERPONE = -1;

// Fin constantes

bool es_posicion_valida(coordenada_t posicion)
{
    return posicion.fil < MAX_FILAS && posicion.fil >= 0 && posicion.col < MAX_COLUMNAS && posicion.col >= 0;
}

bool es_misma_coordenada(coordenada_t cord1, 
                         coordenada_t cord2)
{
    return cord1.col == cord2.col && cord1.fil == cord2.fil;
}

bool posicion_superpone_rectangulo(coordenada_t posicion, 
                                   rectangulo_t rectangulo)
{
    bool en_rango_y = posicion.fil <= rectangulo.abajo_der.fil && posicion.fil >= rectangulo.arriba_izq.fil;
    bool en_rango_x = posicion.col <= rectangulo.abajo_der.col && posicion.col >= rectangulo.arriba_izq.col;
    return en_rango_x && en_rango_y;
}

bool posicion_superpone_mesa(const juego_t *juego, 
                             coordenada_t  posicion, 
                             bool          considerar_contorno)
{
    assert(juego != NULL && "juego no puede ser NULL");
    assert(es_posicion_valida(posicion) && "posicion debe ser una posicion valida");

    /// esto funciona tanto para mesas 2x2 como 1x1
    bool superpone = false;
    int i = 0; 
    while (!superpone && i < juego->cantidad_mesas)
    {
        const mesa_t *mesa = &juego->mesas[i++]; 
        coordenada_t arriba_izq = mesa->posicion[0];
        coordenada_t abajo_der = mesa->posicion[mesa->cantidad_lugares-1];

        rectangulo_t rect_mesa = considerar_contorno ? (rectangulo_t){
            .arriba_izq = {.col=arriba_izq.col-1, .fil=arriba_izq.fil-1},
            .abajo_der = {.col=abajo_der.col+1, .fil=abajo_der.fil+1}
        }:(rectangulo_t){
            .arriba_izq = arriba_izq,
            .abajo_der = abajo_der
        };
        superpone = posicion_superpone_rectangulo(posicion, rect_mesa);
    }
    return superpone;
}

int posicion_superpone_herramienta(const juego_t *juego, 
                                   coordenada_t  posicion,
                                   bool          considerar_mopa)
{
    assert(juego != NULL && "juego no puede ser NULL");
    assert(es_posicion_valida(posicion) && "posicion debe ser una posicion valida");
    bool ocupado = false; 
    int i = 0;
    while (!ocupado && i < juego->cantidad_herramientas)
    {
        const objeto_t *herramienta = &juego->herramientas[i++];
        switch (herramienta->tipo)
        {
            case OBJ_MONEDA:
            case OBJ_PATIN:
                ocupado = es_misma_coordenada(posicion, herramienta->posicion);     
                break;
            case OBJ_MOPA:
                if (considerar_mopa)
                    ocupado = es_misma_coordenada(posicion, herramienta->posicion);
                break;
        }
    }
    return ocupado ? i-1 : NO_SUPERPONE;
}                                

int posicion_superpone_obstaculo(const juego_t *juego, 
                                 coordenada_t  posicion)
{
    assert(juego != NULL && "juego no puede ser NULL");
    assert(es_posicion_valida(posicion) && "posicion debe ser una posicion valida");
    int i = 0;
    bool ocupado = false;
    while (!ocupado && i < juego->cantidad_obstaculos)
    {
        const objeto_t *obstaculo = &juego->obstaculos[i++];
        switch (obstaculo->tipo)
        {
            case OBJ_CHARCO:
                ocupado = es_misma_coordenada(posicion, obstaculo->posicion);  
                break;
            //TODO implementaria cucaracha pero no se el caracter 
        }
    }
    return ocupado ? i-1 : NO_SUPERPONE;
}

bool es_posicion_ocupada(const juego_t *juego, 
                         coordenada_t  posicion,
                         bool          considerar_contorno_mesas,
                         bool          considerar_mozo,
                         bool          considerar_cocina)
{
    assert(juego != NULL && "juego no puede ser NULL");
    assert(es_posicion_valida(posicion) && "posicion debe ser una posicion valida");
    bool ocupado = (considerar_cocina && es_misma_coordenada(posicion, juego->cocina.posicion)) || (considerar_mozo && es_misma_coordenada(posicion, juego->mozo.posicion));
    
    if (!ocupado)
        ocupado = posicion_superpone_mesa(juego, posicion, considerar_contorno_mesas);

    if (!ocupado) 
        ocupado = posicion_superpone_herramienta(juego, posicion, true) != NO_SUPERPONE;

    if (!ocupado)
        ocupado = posicion_superpone_obstaculo(juego, posicion) != NO_SUPERPONE;

    return ocupado;
}

coordenada_t generar_posicion_libre(const juego_t *juego,
                                    generador_t   *generador,
                                    bool          considerar_contorno_mesas,
                                    bool          considerar_mozo,
                                    bool          considerar_cocina)
{
    assert(juego != NULL && "juego no puede ser NULL");
    coordenada_t posicion_aleatoria;
    bool posicion_invalida;
    do
    {
        posicion_aleatoria = generador ? generador_posicion_unica(generador) : generador_posicion_aleatoria();
        posicion_invalida = es_posicion_ocupada(juego, posicion_aleatoria, considerar_contorno_mesas, considerar_mozo, considerar_cocina);
    } while (posicion_invalida);
    return posicion_aleatoria;
}

bool es_mesa_valida(juego_t      *juego, 
                    const mesa_t *mesa,
                    bool         considerar_mozo,
                    bool         considerar_cocina)
{
    assert(juego != NULL && "juego no puede ser NULL");
    assert(mesa != NULL && "mesa no puede ser NULL");
    bool en_terreno = mesa_en_terreno(mesa);
    bool sillas_validas = true;
    if (en_terreno)
    {
        int i = 0;
        while (sillas_validas && i < mesa->cantidad_lugares)
            sillas_validas = !es_posicion_ocupada(juego, mesa->posicion[i++], true, considerar_mozo, considerar_cocina);
    }
    
    return en_terreno && sillas_validas;
}

bool mesa_en_terreno(const mesa_t *mesa)
{
    assert(mesa != NULL && "mesa no puede ser NULL");
    int i = 0;
    bool mesa_en_terreno = true;
    while (mesa_en_terreno && i < mesa->cantidad_lugares)
        mesa_en_terreno = es_posicion_valida(mesa->posicion[i++]);
    return mesa_en_terreno;
}


mesa_t generar_mesa_tentativa(juego_t      *juego, 
                              generador_t  *generador, 
                              tipo_mesas_t tipo_mesa,
                              bool         considerar_mozo,
                              bool         considerar_cocina)
{
    assert(juego != NULL && "juego no puede ser NULL");
    coordenada_t posicion = generar_posicion_libre(juego, generador, true, considerar_mozo, considerar_cocina);
    mesa_t mesa_tentativa;

    if (tipo_mesa == MESAS_1X1)
        mesa_tentativa = (mesa_t){.cantidad_lugares=1, .posicion[0]=posicion};
    else if (tipo_mesa == MESAS_2X2)
        // inicializar los 4 tiles, en el orden de derecha a izquierda y de  
        // arriba a abajo. posicion_superpone_mesa() depende de este orden
        mesa_tentativa = (mesa_t){
            .cantidad_lugares = 4,
            .posicion[0] = posicion,
            .posicion[1] = (coordenada_t){.fil= posicion.fil, .col= posicion.col+1},
            .posicion[2] = (coordenada_t){.fil= posicion.fil+1, .col= posicion.col},
            .posicion[3] = (coordenada_t){.fil= posicion.fil+1, .col= posicion.col+1}
        };
    else
        assert(false && "tipo_mesa es invalido");

    return mesa_tentativa;
}

void eliminar_objeto(objeto_t *objetos, int *cantidad_objetos, int indice_a_eliminar)
{
    assert(objetos != NULL && "objetos no puede ser NULL");
    for (int i = indice_a_eliminar; i < *cantidad_objetos-1; i++)
    {
        objetos[i] = objetos[i+1];
    }
    objetos[*cantidad_objetos-1] = (objeto_t){0};

    (*cantidad_objetos)--;
}

uint64_t calcular_distancia_manhattan(coordenada_t cord1, coordenada_t cord2)
{
    return (uint64_t)abs(cord1.fil - cord2.fil) + (uint64_t)abs(cord1.col - cord2.col);
}