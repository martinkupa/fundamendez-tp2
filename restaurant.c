#include "restaurant.h"
#include "restaurant_utils.h"
#include "restaurant_io.h"
#include "vector_pedidos.h"
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

/// @brief Interactua con todos los objetos del juego segun corresponda
/// @pre `juego` no debe ser NULL
static void interactuar_con_objetos(juego_t *juego);

/// @brief Contabiliza la cantidad de cucarachas dentro del rango de 
///        penalizacion de `mesa`
/// @pre `juego` no debe ser NULL
/// @pre `mesa` no debe ser NULL
static int cantidad_cucarachas_cerca(const juego_t *juego, 
                                     const mesa_t  *mesa);

/// @brief Disminuye la paciencia de las mesas ocupadas y las libera cuando esta
///        se agota
/// @pre `juego` no debe ser NULL
static void disminuir_paciencia_comensales(juego_t *juego);

/// @brief Cocina los pedidos en preparacion y una vez listos los transfiere a
///        cocina_t::platos_listos
/// @pre cocina no debe ser NULL
static void cocinar_platillos(cocina_t *cocina);

/// @brief Elimina los pedidos asociados a `indice_mesa` del vector `platillos` 
/// @return true si la operacion fue exitosa, o false si hubo errores al
///         reservar memoria
/// @pre platillos no debe ser NULL
/// @pre cantidad_platillos no debe ser NULL
static bool borrar_platillos_dinamicos_por_mesa(vector_pedidos_t *platillos,
                                                int              *cantidad_platillos, 
                                                int              indice_mesa);


/// @brief Borra tanto los pedidos como los platillos asociados a `indice_mesa`
///        que tiene el mozo encima
/// @pre mozo no puede ser NULL
static void borrar_pedidos_mozo_por_mesa(mozo_t *mozo, 
                                         int    indice_mesa);

/// @brief Libera una mesa eliminando los comensales y los pedidos asociados a 
///        estos
/// @param indice_mesa El indice de la mesa a eliminar en el vector 
///        juego_t::mesas
/// @pre juego no debe ser NULL
/// @pre indice_mesa debe estar en el rango de [0, juego->cantidad_mesas)
static void eliminar_comensales(juego_t *juego,
                                int     indice_mesa);

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
            fprintf(stderr, "[WARN]: realizar_jugada() recibio una jugada invalida\n");
            return;
    }

    if (jugada_realizada)
    {
        juego->movimientos++;
        disminuir_paciencia_comensales(juego);
        cocinar_platillos(&juego->cocina);
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

    mozo_t *mozo = &juego->mozo;
    int indice_obstaculo = posicion_superpone_obstaculo(juego, mozo->posicion);
    if (indice_obstaculo != NO_SUPERPONE)
        interactuar_con_obstaculo(juego, indice_obstaculo);

    if (mozo->tiene_mopa)
        return; 

    interactuar_con_mesas(juego); 

    int indice_herramienta = posicion_superpone_herramienta(juego, mozo->posicion, false);
    if (indice_herramienta != NO_SUPERPONE)
        interactuar_con_herramienta(juego, indice_herramienta);       

    bool interactuar_cocina = es_misma_coordenada(mozo->posicion, juego->cocina.posicion);
    if (interactuar_cocina)
        interactuar_con_cocina(mozo, &juego->cocina); 
}

static int cantidad_cucarachas_cerca(const juego_t *juego, 
                                     const mesa_t  *mesa)
{
    assert(juego != NULL && "juego no puede ser NULL");
    assert(mesa != NULL && "mesa no puede ser NULL");

    int cantidad_cucarachas = 0;
    for (int i = 0; i < juego->cantidad_obstaculos; i++)
    {
        const objeto_t *obstaculo = &juego->obstaculos[i++];
        if (obstaculo->tipo == OBJ_CUCARACHA)
            cantidad_cucarachas += posicion_dentro_rango_mesa(obstaculo->posicion, mesa, RANGO_CUCARACHAS);
    }

    return cantidad_cucarachas; 
}

static void disminuir_paciencia_comensales(juego_t *juego)
{
    for (int i = 0; i < juego->cantidad_mesas; i++)
    {
        mesa_t *mesa = &juego->mesas[i];
        bool hay_comensales = mesa->cantidad_comensales > 0;
        if (hay_comensales)
            mesa->paciencia -= 1 + PENALIZACION_CUCARACHA * cantidad_cucarachas_cerca(juego, mesa);
        
        bool paciencia_agotada = mesa->paciencia <= 0;
        if (paciencia_agotada)
            eliminar_comensales(juego, i); 
    }
    
}

static bool borrar_platillos_dinamicos_por_mesa(vector_pedidos_t *platillos,
                                                int              *cantidad_platillos, 
                                                int              indice_mesa)
{
    assert(platillos != NULL && "platillos no puede ser NULL");
    assert(cantidad_platillos != NULL && "cantidad_platillos no debe ser NULL");
    for (int i = 0; i < *cantidad_platillos; i++)
    {
        pedido_t *platillo_preparacion = &(*platillos)[i];
        if (platillo_preparacion->id_mesa == indice_mesa)
        {
            vector_pedidos_t nuevo_vector = eliminar_pedido_dinamico(*platillos, cantidad_platillos, i); 
            bool sin_memoria = nuevo_vector == NULL && *cantidad_platillos != 0;
            if (sin_memoria)
                return false;
            *platillos = nuevo_vector; 
        }
    }
    return true;
}

static void borrar_pedidos_mozo_por_mesa(mozo_t *mozo, 
                                         int    indice_mesa)
{
    assert(mozo != NULL && "mozo no puede ser NULL");
    for (int i = 0; i < mozo->cantidad_pedidos; i++)
    {
        pedido_t *pedido = &mozo->pedidos[i];
        if (pedido->id_mesa == indice_mesa)
            eliminar_pedido(mozo->pedidos, &mozo->cantidad_pedidos, i);
    }

    for (int i = 0; i < mozo->cantidad_bandeja; i++)
    {
        pedido_t *platillo = &mozo->bandeja[i];
        if (platillo->id_mesa == indice_mesa)
            eliminar_pedido(mozo->bandeja, &mozo->cantidad_bandeja, i);
    }
}

static void eliminar_comensales(juego_t *juego,
                                int     indice_mesa)
{
    assert(juego != NULL && "juego no puede ser NULL");
    assert(indice_mesa >= 0 && indice_mesa < juego->cantidad_mesas && "indice_mesa no esta en rango");
    mesa_t *mesa = &juego->mesas[indice_mesa];
    mesa->cantidad_comensales = 0;
    mesa->paciencia = 0;
    mesa->pedido_tomado = false;

    borrar_pedidos_mozo_por_mesa(&juego->mozo, indice_mesa);     

    cocina_t *cocina = &juego->cocina;
    bool exito = borrar_platillos_dinamicos_por_mesa(&cocina->platos_preparacion, &cocina->cantidad_preparacion, indice_mesa);
    if (!exito)
        terminar_fallo(cocina, "Sin memoria! Terminando...");
 
    exito = borrar_platillos_dinamicos_por_mesa(&cocina->platos_listos, &cocina->cantidad_listos, indice_mesa);
    if (!exito) 
        terminar_fallo(cocina, "Sin memoria! Terminando..."); 
}

static void cocinar_platillos(cocina_t *cocina)
{
    assert(cocina != NULL && "cocina no debe ser NULL");
    for (int i = 0; i < cocina->cantidad_preparacion; i++)
    {
        pedido_t *platillo = &cocina->platos_preparacion[i];
        platillo->tiempo_preparacion--;
        bool esta_plato_listo = platillo->tiempo_preparacion <= 0;
        if (esta_plato_listo)
        {   
            pedido_t plato_listo = *platillo;
            vector_pedidos_t nuevo_vector_preparacion = eliminar_pedido_dinamico(cocina->platos_preparacion, &cocina->cantidad_preparacion, i);

            bool sin_memoria = nuevo_vector_preparacion == NULL && cocina->cantidad_preparacion != 0;
            if (sin_memoria)
                terminar_fallo(cocina, "Sin memoria! Terminando...");

            cocina->platos_preparacion = nuevo_vector_preparacion;

            vector_pedidos_t nuevo_vector_listo = agregar_pedido_dinamico(cocina->platos_listos, &cocina->cantidad_listos, plato_listo);

            sin_memoria = nuevo_vector_listo == NULL;
            if (sin_memoria) 
                terminar_fallo(cocina, "Sin memoria! Terminando...");

            cocina->platos_listos = nuevo_vector_listo;            
        }
    } 
}

// Fin funciones estaticas

