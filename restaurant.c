#include "restaurant.h"
#include "restaurant_utils.h"
#include "restaurant_io.h"
#include "generador.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

// Declaraciones de funciones estaticas

// Funciones del TP1

/// @brief inicializa las mesas del juego
/// @param generador si no es NULL, se utilizara para generar posiciones unicas
/// @pre juego no debe ser NULL
static void inicializar_mesas(juego_t     *juego, 
                              generador_t *generador);


/// @brief Inicializa las herramientas del juego
/// @param generador si no es NULL, se utilizara para generar posiciones unicas
/// @pre juego no debe ser NULL
static void inicializar_herramientas(juego_t     *juego, 
                                     generador_t *generador);
/// @brief Iniciliza la mopa
/// @param generador si no es NULL, se utilizara para generar posiciones unicas
/// @pre juego != NULL
static void inicializar_mopa(juego_t     *juego, 
                             generador_t *generador);

/// @brief Inicializa las monedas del juego
/// @param generador si no es NULL, se utilizara para generar posiciones unicas
/// @pre juego != NULL
static void inicializar_monedas(juego_t     *juego, 
                                generador_t *generador);

/// @brief Inicializa los patines del juego
/// @param generador si no es NULL, se utilizara para generar posiciones unicas
/// @pre juego != NULL
static void inicializar_patines(juego_t     *juego, 
                                generador_t *generador);
/// @brief Inicializa los obstaculos del juego
/// @param generador si no es NULL, se utilizara para generar posiciones unicas
/// @pre juego != NULL
static void inicializar_obstaculos(juego_t     *juego, 
                                   generador_t *generador);

/// @brief Intenta mover el juego->mozo a `nueva_pos`
/// @param delta_posicion la variacion de la posicion en cada eje
/// @return Un boolean que indica si el movimiento se realizo exitosamente
/// @pre `juego` no puede ser NULL
static bool mover_linguini(juego_t      *juego, 
                           coordenada_t delta_posicion);

/// @brief Hace que juego->mozo agarre o suelte la mopa, dependiendo de si la 
///        tenia en la mano. Si se suelta la mopa en una posicion invalida, se 
//         ignora el intento. 
/// @pre `juego` no debe ser NULL
static void cambiar_mopa(juego_t *juego);

// Fin funciones del TP1

// TODO implementar y modularizar
//static void interactuar_con_herramienta(juego_t *juego, objeto_t *herramienta);
//static void interactuar_con_obstaculo(juego_t *juego, objeto_t *obstaculo);
// TODO documentar
static void interactuar_con_objetos(juego_t *juego);


// Fin declaraciones estaticas

// Definiciones de funciones

// Funciones publicas

// Funciones del TP1

void inicializar_juego(juego_t *juego) 
{
    // un generador de posiciones unicas
    generador_t generador = generador_constructor();

    // inicializo a cero para evitar iterar sobre arrays no inicializados al
    // generar posiciones
    juego->cantidad_mesas = 0;
    juego->cantidad_obstaculos = 0;
    juego->cantidad_herramientas = 0;

    juego->dinero = 0;
    juego->movimientos = 0;

    inicializar_mesas(juego, &generador);

    // init cocina 
    juego->cocina = (cocina_t){
        .posicion = generar_posicion_libre(juego, &generador, false, false, false)
    };

    // init linguini
    juego->mozo = (mozo_t){
        .posicion = generar_posicion_libre(juego, &generador, false, false, true)
    };

    inicializar_herramientas(juego, &generador);     

    inicializar_obstaculos(juego, &generador);
}

void realizar_jugada(juego_t *juego, char accion)
{
    assert(juego != NULL && "juego no puede ser NULL");
    
    bool jugada_realizada = false;
    switch (accion)
    {
        case ACCION_ARRIBA:
            jugada_realizada = mover_linguini(juego, (coordenada_t){.fil=-1,.col=0});
            break;

        case ACCION_DERECHA:
            jugada_realizada = mover_linguini(juego, (coordenada_t){.fil=0,.col=+1});
            break;

        case ACCION_ABAJO:
            jugada_realizada = mover_linguini(juego, (coordenada_t){.fil=+1,.col=0});
            break;

        case ACCION_IZQUIERDA:
            jugada_realizada = mover_linguini(juego, (coordenada_t){.fil=0,.col=-1}); 
            break;

        case ACCION_MOPA:
            cambiar_mopa(juego);
            break;

        case ACCION_PATIN:
            bool puede_usar_patin = juego->mozo.cantidad_patines > 0 && !juego->mozo.patines_puestos;
            if (puede_usar_patin)
            {
                juego->mozo.cantidad_patines--;
                juego->mozo.patines_puestos = true;
            }
            break; 
        default:
            assert(false && "Recibi una accion no valida");
            fprintf(stderr, "[WARN]: realizar_jugada recibio una jugada invalida\n");
            return;
    }

    if (jugada_realizada)
    {
        juego->movimientos++;
        spawnear_entidades(juego); 
    }
}

void mostrar_juego(juego_t juego)
{
    char buffer[MAX_FILAS*(MAX_COLUMNAS+1)+1];
    const uint16_t LARGO_BUF = MAX_FILAS*(MAX_COLUMNAS+1)+1;

    construir_string_terreno_juego(&juego, LARGO_BUF, buffer);

    unsigned int cantidad_comensales = calcular_comensales(&juego);
    printf("\n%s\n"
            "%s%-20d%s%-20d%s%-20d%s%-20d\n"
            "%s%-16d%s%-20u\n"
            "%s%-14d%s%-20d\n",
            buffer, 
            "Movimientos: ", juego.movimientos, "Dinero: ", juego.dinero, "Patines: ", juego.mozo.cantidad_patines, "Comensales: ", cantidad_comensales,
            "Pedidos tomados: ", juego.mozo.cantidad_pedidos, "Platos en bandeja: ", juego.mozo.cantidad_bandeja, 
            "Pedidos en cocina: ", juego.cocina.cantidad_preparacion, "Platos en cocina: ", juego.cocina.cantidad_listos);

    if (juego.mozo.tiene_mopa)
        printf("Tienes la mopa en mano\n");

    if (juego.mozo.patines_puestos)
        printf("Tienes los patines puestos\n"); 
}

int estado_juego(juego_t juego)
{
    bool juego_termino = juego.movimientos >= LIMITE_MOVIMIENTOS;
    bool juego_ganado = juego.dinero >= OBJETIVO_DINERO;

    if (!juego_termino)
        return CONTINUA;

    return juego_ganado ? GANO : PERDIO;
}

// Fin funciones del TP1

void destruir_juego(juego_t *juego)
{
    assert(juego != NULL && "juego no puede ser NULL");
    free(juego->cocina.platos_listos);
    juego->cocina.platos_listos = NULL;
    free(juego->cocina.platos_preparacion);
    juego->cocina.platos_preparacion = NULL;
}

// Fin funciones publicas

// Funciones estaticas

// Funciones del TP1

static void inicializar_mesas(juego_t     *juego, 
                              generador_t *generador)
{
    assert(juego != NULL && "juego no puede ser NULL");
    juego->cantidad_mesas = 0;
    
    for (int i = 0; i < CANTIDAD_MESAS_2X2; i++)
    {
        bool mesa_valida;
        mesa_t mesa_tentativa;
        do
        {
            mesa_tentativa = generar_mesa_tentativa(juego, generador, MESAS_2X2, false, false);
            mesa_valida = es_mesa_valida(juego, &mesa_tentativa, false, false); 
        } while (!mesa_valida);
        juego->mesas[juego->cantidad_mesas] = mesa_tentativa;
        juego->cantidad_mesas++;
    } 

    for (int i = 0; i < CANTIDAD_MESAS_1X1; i++)
    {
        // al ser una mesa 1x1 sabemos que sera valida de antemano
        juego->mesas[juego->cantidad_mesas] = generar_mesa_tentativa(juego, generador, MESAS_1X1, false, false);
        juego->cantidad_mesas++;
    }
}

static void inicializar_mopa(juego_t     *juego, 
                             generador_t *generador)
{
    assert(juego != NULL && "El juego no debe ser NULL");
    juego->herramientas[juego->cantidad_herramientas] = (objeto_t){
        .tipo = OBJ_MOPA, 
        .posicion = generar_posicion_libre(juego, generador, false, true, true)
    };
    juego->cantidad_herramientas++;
}

static void inicializar_monedas(juego_t     *juego, 
                                generador_t *generador)
{
    assert(juego != NULL && "El juego no debe ser NULL");
    int inicio = juego->cantidad_herramientas;
    while (juego->cantidad_herramientas < inicio + CANTIDAD_MONEDAS)
    {
        juego->herramientas[juego->cantidad_herramientas] = (objeto_t){
            .tipo = OBJ_MONEDA,
            .posicion = generar_posicion_libre(juego, generador, false, true, true)
        };
        juego->cantidad_herramientas++;
    }
}

static void inicializar_patines(juego_t     *juego, 
                                generador_t *generador)
{
    assert(juego != NULL && "El juego no debe ser NULL");
    int inicio = juego->cantidad_herramientas;
    while (juego->cantidad_herramientas < inicio + CANTIDAD_PATINES)
    {
        juego->herramientas[juego->cantidad_herramientas] = (objeto_t){
            .tipo = OBJ_PATIN,
            .posicion = generar_posicion_libre(juego, generador, false, true, true)
        };
        juego->cantidad_herramientas++;
    }
}


static void inicializar_herramientas(juego_t   *juego, 
                                     generador_t *generador)
{
    assert(juego != NULL && "juego no debe ser NULL");
    juego->cantidad_herramientas = 0;
    inicializar_mopa(juego, generador);
    inicializar_monedas(juego, generador);   
    inicializar_patines(juego, generador); 
}

static void inicializar_obstaculos(juego_t     *juego, 
                                   generador_t *generador)
{
    assert(juego != NULL && "juego no debe ser NULL");
    juego->cantidad_obstaculos = 0;

    //init charchos
    while (juego->cantidad_obstaculos < CANTIDAD_CHARCOS)
    {
        juego->obstaculos[juego->cantidad_obstaculos] = (objeto_t){
            .tipo = OBJ_CHARCO,
            .posicion = generar_posicion_libre(juego, generador, false, true, true)
        };
        juego->cantidad_obstaculos++;
    }
}

static bool mover_linguini(juego_t      *juego, 
                           coordenada_t delta_posicion)
{
    assert(juego != NULL);
    bool movimiento_exitoso = false;
    do
    {
        coordenada_t nueva_posicion = {
            .fil = juego->mozo.posicion.fil + delta_posicion.fil,
            .col = juego->mozo.posicion.col + delta_posicion.col
        };

        if (!es_posicion_valida(nueva_posicion) || posicion_superpone_mesa(juego, nueva_posicion, false))
        {
            juego->mozo.patines_puestos = false;
        } else
        {
            juego->mozo.posicion = nueva_posicion;
            movimiento_exitoso = true;
            interactuar_con_objetos(juego);
        }
    } while (juego->mozo.patines_puestos);
 
    return movimiento_exitoso;
}

static void cambiar_mopa(juego_t *juego)
{
    // TODO: implementar eliminado "fisico"
    // estoy asumiendo que en el juego solo hay una unica mopa
    if (juego->mozo.tiene_mopa)
    {
        bool posicion_invalida = es_misma_coordenada(juego->mozo.posicion, juego->cocina.posicion) || posicion_superpone_mesa(juego, juego->mozo.posicion, false) || posicion_superpone_herramienta(juego, juego->mozo.posicion, false) != NO_SUPERPONE || posicion_superpone_obstaculo(juego, juego->mozo.posicion) != NO_SUPERPONE;
        if (posicion_invalida)
            return;
        
        juego->mozo.tiene_mopa = false;
        assert(juego->herramientas[INDICE_MOPA].tipo == OBJ_MOPA && "La mopa debe estar en la posicion 0");
        juego->herramientas[INDICE_MOPA].posicion = juego->mozo.posicion;
    } else 
    {
        assert(juego->herramientas[INDICE_MOPA].tipo == OBJ_MOPA && "La mopa debe estar en la posicion 0");
        coordenada_t posicion_mopa = juego->herramientas[INDICE_MOPA].posicion;
        if (es_misma_coordenada(juego->mozo.posicion, posicion_mopa))
        {
            juego->herramientas[INDICE_MOPA].posicion = (coordenada_t){.col=0, .fil=0};
            juego->mozo.tiene_mopa = true;
        } 
    } 
}

// Fin funciones del TP1

static void interactuar_con_objetos(juego_t *juego)
{
    assert(juego != NULL && "juego no puede ser NULL");
    interactuar_con_mesas(juego);    

    mozo_t *mozo = &juego->mozo;
    int indice_herramienta = posicion_superpone_herramienta(juego, mozo->posicion, false);
    if (indice_herramienta != NO_SUPERPONE)
    {
        const objeto_t *herramienta = &juego->herramientas[indice_herramienta];
        switch (herramienta->tipo)
        {
            case OBJ_PATIN:
                mozo->cantidad_patines++;
                eliminar_objeto(juego->herramientas, &juego->cantidad_herramientas, indice_herramienta);
                break;
            
            case OBJ_MONEDA:
                juego->dinero += 1000;
                eliminar_objeto(juego->herramientas, &juego->cantidad_herramientas, indice_herramienta);
                break;
            
        }
    }

    int indice_obstaculo = posicion_superpone_obstaculo(juego, mozo->posicion);
    if (indice_obstaculo != NO_SUPERPONE)
    {
        const objeto_t *obstaculo = &juego->obstaculos[indice_obstaculo];
        switch (obstaculo->tipo)
        {
            case OBJ_CHARCO:
                if (mozo->tiene_mopa)
                    eliminar_objeto(juego->obstaculos, &juego->cantidad_obstaculos, indice_obstaculo);
                else
                    /*TODO: perder platos*/{} 
                break;
        }
    }

    bool interactuar_cocina = es_misma_coordenada(mozo->posicion, juego->cocina.posicion);
    if (interactuar_cocina)
        interactuar_con_cocina(mozo, &juego->cocina); 
}


// Fin funciones estaticas

