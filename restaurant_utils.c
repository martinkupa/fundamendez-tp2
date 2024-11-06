#include "restaurant_utils.h"
#include "vector_pedidos.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


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
const uint8_t COOLDOWN_CUCARACHAS = 25;
const uint8_t COOLDOWN_COMENSALES = 15;
const uint8_t COMENSALES_MINIMO = 1;
const uint8_t COMENSALES_MAXIMO = 4;
const uint8_t PENALIZACION_CUCARACHA = 2;
const uint8_t RANGO_CUCARACHAS = 2;
const unsigned int ALCANCE_MOZO = 1;


const struct {
    char plato;
    int tiempo_preparacion;
} MENU[] = {
    [0] = {.plato=PLATO_NAPOLITANA, .tiempo_preparacion=30},
    [1] = {.plato=PLATO_HAMBURGUESA, .tiempo_preparacion=15},
    [2] = {.plato=PLATO_PARRILLA, .tiempo_preparacion=20},
    [3] = {.plato=PLATO_RATATOUILLE, .tiempo_preparacion=25}
};

// Fin constantes

/// @brief Comprueba si el mozo esta en el rango como para interactuar con la 
///        mesa
/// @pre mozo no puede ser NULL
/// @pre mesa no puede ser NULL
static bool mozo_alcanza_mesa(const mozo_t *mozo, const mesa_t *mesa);

// Funciones del TP1

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

// Fin funciones del TP1

uint64_t calcular_distancia_manhattan(coordenada_t cord1, 
                                      coordenada_t cord2)
{
    return (uint64_t)abs(cord1.fil - cord2.fil) + (uint64_t)abs(cord1.col - cord2.col);
}

pedido_t tomar_pedido(juego_t *juego, 
                      int     indice_mesa)
{
    assert(juego != NULL && "el juego no puede ser NULL");
    assert(indice_mesa >= 0 && indice_mesa < juego->cantidad_mesas && "indice_mesa debe estar en rango");
    pedido_t pedido = {};
    mesa_t *mesa = &juego->mesas[indice_mesa];
    for (int i = 0; i < mesa->cantidad_comensales; i++)
    {
        int plato = generador_numero_aleatorio(0,4);
        pedido.platos[i] = MENU[plato].plato;
        if (pedido.tiempo_preparacion < MENU[plato].tiempo_preparacion)
            pedido.tiempo_preparacion = MENU[plato].tiempo_preparacion; 
    }
    pedido.cantidad_platos = mesa->cantidad_comensales;
    pedido.id_mesa = indice_mesa;
    return pedido;
}


void spawnear_entidades(juego_t *juego)
{
    assert(juego != NULL && "el juego no puede ser NULL");
    bool spawnear_comensales = juego->movimientos % COOLDOWN_COMENSALES == 0;

    if (spawnear_comensales)
    {
        uint8_t cantidad_comensales = (uint8_t)generador_numero_aleatorio(COMENSALES_MINIMO, COMENSALES_MAXIMO + 1);
        mesa_t *mesa_adecuada = buscar_mesa_adecuada(juego, cantidad_comensales);
        if (mesa_adecuada)
        {
            mesa_adecuada->pedido_tomado = false;
            mesa_adecuada->cantidad_comensales = cantidad_comensales; 
            mesa_adecuada->paciencia = generador_numero_aleatorio(PACIENCIA_MINIMA, PACIENCIA_MAXIMA + 1); 
        } 
    }

    bool spawnear_cucarachas = juego->movimientos % COOLDOWN_CUCARACHAS == 0;
    if (spawnear_cucarachas)
    {
        // TODO: spawn cucarachas
    } 
}

mesa_t *buscar_mesa_adecuada(juego_t      *juego, 
                             unsigned int cantidad_comensales)
{
    assert(juego != NULL && "el juego no puede ser NULL");
    mesa_t *mejor_mesa = NULL;
    int64_t asientos_libres_mejor_mesa = INT64_MAX;
    for (int i = 0; i < juego->cantidad_mesas; i++)
    {
        mesa_t *mesa_actual = &juego->mesas[i];
        bool mesa_ocupada = mesa_actual->cantidad_comensales > 0;
        if (!mesa_ocupada)
        {
            int asientos_libres = mesa_actual->cantidad_lugares - (int)cantidad_comensales;
            bool hay_lugar_para_todos = asientos_libres >= 0;
            bool es_mejor_mesa = hay_lugar_para_todos && asientos_libres < asientos_libres_mejor_mesa;
            if (es_mejor_mesa)
            {
                asientos_libres_mejor_mesa = asientos_libres;
                mejor_mesa = mesa_actual;
            } 
        } 
    }
    return mejor_mesa;
}

unsigned int calcular_comensales(const juego_t *juego)
{
    uint8_t cantidad_comensales = 0;
    for (int i = 0; i < juego->cantidad_mesas; i++)
        cantidad_comensales += (uint8_t)juego->mesas[i].cantidad_comensales;
    return cantidad_comensales;
}

void interactuar_con_cocina(mozo_t   *mozo, 
                            cocina_t *cocina)
{
    if (mozo->tiene_mopa)
        return;

    for (int i = mozo->cantidad_pedidos-1; i >= 0; i--)
    {
        vector_pedidos_t nuevo_vector = agregar_pedido_dinamico(cocina->platos_preparacion, &cocina->cantidad_preparacion, mozo->pedidos[i]);
        if (!nuevo_vector)
            terminar_fallo(cocina, "Sin memoria! Terminando...");
        else 
            cocina->platos_preparacion = nuevo_vector;
        eliminar_pedido(mozo->pedidos, &mozo->cantidad_pedidos, i);
    }

    bool cocina_agotada = cocina->cantidad_listos <= 0;
    bool bandeja_llena = mozo->cantidad_bandeja >= MAX_BANDEJA;
    while (!cocina_agotada && !bandeja_llena)
    {
        pedido_t pedido_listo = cocina->platos_listos[cocina->cantidad_listos-1];
        vector_pedidos_t nuevo_vector = eliminar_pedido_dinamico(cocina->platos_listos, &cocina->cantidad_listos, cocina->cantidad_listos-1);
        bool sin_memoria = !nuevo_vector && cocina->cantidad_listos != 0;
        if (sin_memoria)
            terminar_fallo(cocina, "Sin memoria! Terminando...");
        cocina->platos_listos = nuevo_vector;
        mozo->bandeja[mozo->cantidad_bandeja++] = pedido_listo;

        cocina_agotada = cocina->cantidad_listos <= 0;
        bandeja_llena = mozo->cantidad_bandeja >= MAX_BANDEJA;
    }   
}

void interactuar_con_mesas(juego_t *juego)
{
    assert(juego != NULL && "el juego no puede ser NULL");
    mozo_t *mozo = &juego->mozo;
    for (int i = 0; i < juego->cantidad_mesas; i++)
    {
        mesa_t *mesa = &juego->mesas[i];
        bool hay_comensales = mesa->cantidad_comensales > 0;
        bool espacio_pedido_nuevo = mozo->cantidad_pedidos < MAX_PEDIDOS;
        if (hay_comensales && mozo_alcanza_mesa(mozo, mesa))
        {
            if (mesa->pedido_tomado)
            {}
            else if (espacio_pedido_nuevo)
            {
                pedido_t pedido = tomar_pedido(juego, i);
                mozo->pedidos[mozo->cantidad_pedidos++] = pedido;
                mesa->pedido_tomado = true;
            }
        }
    }
}

bool posicion_dentro_rango_mesa(coordenada_t coordenada, 
                                const mesa_t *mesa,
                                unsigned int rango)
{
    assert(mesa != NULL && "mesa no puede ser NULL");
    bool en_rango = false;
    int i = 0;
    while (!en_rango && i < mesa->cantidad_lugares)
        en_rango = calcular_distancia_manhattan(coordenada, mesa->posicion[i++]) <= rango;
    return en_rango;
}

void terminar_fallo(cocina_t *cocina, const char *mensaje)
{
    if (cocina)
    {
        free(cocina->platos_preparacion);
        cocina->platos_preparacion = NULL;
        free(cocina->platos_listos);
        cocina->platos_listos = NULL;
    }
    
    fprintf(stderr, "[FATAL]: %s\n", mensaje);
    exit(1);
}

static bool mozo_alcanza_mesa(const mozo_t *mozo, const mesa_t *mesa)
{
    assert(mozo != NULL && "mozo no puede ser NULL");
    assert(mesa != NULL && "mesa no puede ser NULL");
    return posicion_dentro_rango_mesa(mozo->posicion, mesa, ALCANCE_MOZO); 
}

