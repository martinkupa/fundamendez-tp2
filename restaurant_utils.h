#ifndef __RESTAURANT_UTILS_H__
#define __RESTAURANT_UTILS_H__

#include "generador.h"
#include "restaurant.h"
#include "vector_pedidos.h"
#include "vector_operaciones.h"

#include <inttypes.h>

#define OBJ_LINGUINI 'L'
#define OBJ_MESA 'T'
#define OBJ_COCINA 'C'
#define OBJ_CHARCO 'H'
#define OBJ_MOPA 'O'
#define OBJ_MONEDA 'M'
#define OBJ_PATIN 'P'
#define OBJ_COMENSAL 'X'
#define OBJ_CUCARACHA 'U'
#define ACCION_ARRIBA 'W'
#define ACCION_DERECHA 'D'
#define ACCION_ABAJO 'S'
#define ACCION_IZQUIERDA 'A'
#define ACCION_MOPA 'O'
#define ACCION_PATIN 'P'
#define CANTIDAD_ACCIONES 6
#define PLATO_NAPOLITANA 'M'
#define PLATO_HAMBURGUESA 'H'
#define PLATO_PARRILLA 'P'
#define PLATO_RATATOUILLE 'R'

typedef struct rectangulo {
    coordenada_t arriba_izq;
    coordenada_t abajo_der;
} rectangulo_t;

// Typedef para que la API sea mas expresiva

/// @brief Los tipos de mesas que el juego permite
typedef uint8_t tipo_mesas_t;

extern const tipo_mesas_t MESAS_1X1;
extern const tipo_mesas_t MESAS_2X2;
extern const uint16_t PAGO_COMENSAL;
extern const int8_t CANTIDAD_MESAS_1X1;
extern const int8_t CANTIDAD_MESAS_2X2;
extern const uint8_t CANTIDAD_CHARCOS;
extern const int32_t OBJETIVO_DINERO;
extern const uint8_t LIMITE_MOVIMIENTOS;
extern const int GANO;
extern const int CONTINUA;
extern const int PERDIO;
extern const uint8_t CANTIDAD_PATINES;
extern const uint8_t CANTIDAD_MONEDAS;
extern const uint8_t INDICE_MOPA;
extern const uint8_t PACIENCIA_MINIMA;
extern const uint8_t PACIENCIA_MAXIMA;
extern const int NO_SUPERPONE;
extern const uint8_t COOLDOWN_CUCARACHAS;
extern const uint8_t COOLDOWN_COMENSALES;
extern const uint8_t COMENSALES_MINIMO;
extern const uint8_t COMENSALES_MAXIMO;
extern const uint8_t PENALIZACION_CUCARACHA;
extern const uint8_t RANGO_CUCARACHAS;
extern const unsigned int ALCANCE_MOZO;

bool es_misma_coordenada(coordenada_t cord1, 
                         coordenada_t cord2);

/// @brief comprueba si `posicion` esta dentro de los limites del terreno de 
///        juego
bool es_posicion_valida(coordenada_t posicion);

/// @brief Revisa si una mesa entra dentro del campo de juego
/// @pre mesa != NULL
bool mesa_en_terreno(const mesa_t *mesa);

/// @brief Revisa si `posicion` esta dentro del area del `rectangulo`. El origen
///        esta arriba a la derecha.
bool posicion_superpone_rectangulo(coordenada_t posicion, 
                                   rectangulo_t rectangulo);

/// @brief Comprueba si `posicion` se superpone con las mesas del juego 
/// @param posicion una posicion valida dentro del juego 
/// @param considerar_contorno indica si considerar el espacio que rodea a la 
///        mesas como parte de estas.
/// @return true si se superpone, false de lo contrario
/// @pre `juego` no puede ser NULL
/// @pre `posicion` debe ser una posicion valida
bool posicion_superpone_mesa(const juego_t *juego, 
                              coordenada_t  posicion, 
                              bool          considerar_contorno);

/// @brief Comprueba si `posicion` se superpone con alguna herramienta y  
///        devuelve su indice
/// @param posicion una posicion valida dentro del juego
/// @param considerar_mopa indica si la posicion de la mopa ha de ser tomada en 
///        cuenta 
/// @return el indice de la herramienta con la que se superpone, o NO_SUPERPONE 
///         en caso contrario
/// @pre juego no puede ser NULL
int posicion_superpone_herramienta(const juego_t *juego, 
                                   coordenada_t  posicion,
                                   bool          considerar_mopa);

/// @brief Comprueba si `posicion` se superpone con algun obstaculo, en cuyo 
///        caso devuelve el indice
/// @param posicion una posicion valida
/// @returns el indice del obstaculo que se superponer o NO_SUPERPONE de lo 
///          contrario
/// @pre juego no debe ser NULL
int posicion_superpone_obstaculo(const juego_t *juego, 
                                 coordenada_t  posicion);

/// @brief Comprueba si una posicion esta ocupada por algun elemento
/// @param juego el juego en el que evaluar el terreno
/// @param posicion una posicion valida dentro del juego
/// @param considerar_contorno_mesas indica si el contorno de las mesas debe
///        considerarse como parte de estas 
/// @pre `juego` no puede ser NULL
/// @pre posicion debe ser una posicion valida
bool es_posicion_ocupada(const juego_t *juego, 
                         coordenada_t  posicion, 
                         bool          considerar_contorno_mesas,
                         bool          considerar_mozo,
                         bool          considerar_cocina,
                         bool          considerar_mopa);
                    
/// @brief genera una posicion aleatoria dentro del campo de juego que no este
///        ocupada por ningun elemento
/// @param generador si no es NULL, se utilizara el generador para obtener 
///        posiciones aleatorias unicas. 
/// @param considerar_contorno_mesas indica si el contorno de las mesas debe
///        considerarse como parte de estas 
/// @pre juego no puede ser NULL
/// @return la posicion generada
coordenada_t generar_posicion_libre(const juego_t *juego,
                                    generador_t   *generador,
                                    bool          considerar_contorno_mesas,
                                    bool          considerar_mozo,
                                    bool          considerar_cocina,
                                    bool          considerar_mopa);

/// @brief Revisa si `mesa` es valida, i.e si se superpone con algun
///        otro objeto o esta fuera del mapa 
/// @pre juego != NULL
/// @pre mesa != NULL
bool es_mesa_valida(juego_t      *juego, 
                    const mesa_t *mesa,
                    bool         considerar_mozo,
                    bool         considerar_cocina,
                    bool         considerar_mopa);

/// @brief Genera una mesa tentativa en una posicion aleatoria.
/// @param tipo_mesa El tipo de mesa a generar
/// @param generador si no es NULL, se utilizara el generador para obtener 
///        posiciones aleatorias unicas. 
/// @return La mesa generada, la cual puede o no ser una mesa valida
/// @pre juego != NULL
/// @pre tipo_mesa debe ser uno de los tipos permitidos en el juego
mesa_t generar_mesa_tentativa(juego_t      *juego, 
                              generador_t  *generador, 
                              tipo_mesas_t tipo_mesa,
                              bool         considerar_mozo,
                              bool         considerar_cocina,
                              bool         considerar_mopa);

// Funciones TP 2

/// @brief Calcula la distancia manhattan entre dos puntos
uint64_t calcular_distancia_manhattan(coordenada_t cord1, 
                                      coordenada_t cord2);

/// @brief Genera el pedido para la mesa indicada por el `indice_mesa`.
/// @param indice_mesa El indice de la mesa en el vector juego_t::mesas a la 
///        cual tomar el pedido
/// @note Para mesas sin comensales se devolvera un pedido vacio, excepto por el
///       miembro pedido_t::id_mesa que sera inicializado al valor de 
///       `indice_mesa`
/// @pre juego no puede ser NULL
/// @pre indice_mesa debe estar en rango para el vector juego_t::mesas, 
///      [0, juego->cantidad_mesas)
pedido_t tomar_pedido(juego_t *juego, 
                      int     indice_mesa);

/// @brief Spawnea entidades en el campo de juego en base a los movimientos
///        realizados
/// @pre juego no puede ser NULL
void spawnear_entidades(juego_t *juego);

/// @brief Dada la `cantidad_comensales`, busca la mesa mas adecuada para el
///        grupo. Por adecuada se entiende a la mesa libre cuya cantidad de 
///        lugares sea la minima posible para ubicar a todos los comensales
/// @return Un puntero a la mesa encontrada, o NULL en caso de que hubiese tal
///         mesa.
/// @pre juego no puede ser NULL
mesa_t *buscar_mesa_adecuada(juego_t      *juego, 
                             unsigned int cantidad_comensales);

/// @return La cantidad de comensales esperando su pedido
/// @pre juego no puede ser NULL
unsigned int calcular_comensales(const juego_t *juego);

/// @brief Libera los recursos y termina el programa con un codigo de error y un
///        `mensaje` en stderr
void terminar_fallo(cocina_t   *cocina, 
                    const char *mensaje);

/// @brief Comprueba si la distancia manhattan entre `coordenada` y `mesa` es
///        menor o igual a `rango` (i.e, si esta en rango)
/// @pre mesa no puede ser NULL
bool posicion_dentro_rango_mesa(coordenada_t coordenada, 
                                const mesa_t *mesa,
                                unsigned int rango);

/// @brief Interactua con la mesa y entrega los platillos
/// @pre juego no puede ser NULL
/// @pre indice_mesa debe estar en rango para el vector juego_t::mesas, 
///      [0, juego->cantidad_mesas)
void interactuar_con_mesa(juego_t *juego,
                          int     indice_mesa);

/// @brief Interactua con la cocina. Deja los pedidos a preparar y toma los
///        pedidos preparados
/// @pre mozo no puede ser NULL
/// @pre cocina no puede ser NULL
void interactuar_con_cocina(mozo_t   *mozo, 
                            cocina_t *cocina);

/// @brief Interactua con la herramienta almacenada en la posicion 
///        `indice_herramienta` del vector juego_t::herramientas
/// @pre `juego` no debe ser NULL
/// @pre indice_herramienta debe estar en rango para el vector 
///      juego_t::herramientas, [0, juego->cantidad_herramientas)
void interactuar_con_herramienta(juego_t *juego,
                                 int     indice_herramienta);

/// @brief Interactua con el obstaculo almacenado en la posicion 
///        `indice_obstaculo` del vector juego_t::obstaculos
/// @pre `juego` no debe ser NULL
/// @pre indice_obstaculo debe estar en rango para el vector 
///      juego_t::obstaculos, [0, juego->cantidad_obstaculos)
void interactuar_con_obstaculo(juego_t *juego,
                               int     indice_obstaculo);

/// @brief Libera una mesa eliminando los comensales y los pedidos asociados a 
///        estos
/// @param indice_mesa El indice de la mesa a eliminar en el vector 
///        juego_t::mesas
/// @pre juego no debe ser NULL
/// @pre indice_mesa debe estar en el rango de [0, juego->cantidad_mesas)
void eliminar_comensales(juego_t *juego,
                         int     indice_mesa);

/// @brief Elimina los pedidos asociados a `indice_mesa` del vector `platillos` 
/// @return true si la operacion fue exitosa, o false si hubo errores al
///         reservar memoria
/// @pre platillos no debe ser NULL
/// @pre cantidad_platillos no debe ser NULL
bool borrar_platillos_dinamicos_por_mesa(vector_pedidos_t *platillos,
                                         int              *cantidad_platillos, 
                                         int              indice_mesa);


/// @brief Borra tanto los pedidos como los platillos asociados a `indice_mesa`
///        que tiene el mozo encima
/// @pre mozo no puede ser NULL
void borrar_pedidos_mozo_por_mesa(mozo_t *mozo, 
                                  int    indice_mesa);

/// @brief Comprueba si el mozo esta en el rango como para interactuar con la 
///        mesa
/// @pre mozo no puede ser NULL
/// @pre mesa no puede ser NULL
bool mozo_alcanza_mesa(const mozo_t *mozo, 
                       const mesa_t *mesa);


#endif