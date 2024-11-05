#ifndef __RESTAURANT_UTILS_H__
#define __RESTAURANT_UTILS_H__

#include "generador.h"
#include "restaurant.h"
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
extern const uint16_t PAGO_MESA_1X1;
extern const uint16_t PAGO_MESA_2X2;
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
                         bool          considerar_cocina);
                    
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
                                    bool          considerar_cocina);

/// @brief Revisa si `mesa` es valida, i.e si se superpone con algun
///        otro objeto o esta fuera del mapa 
/// @pre juego != NULL
/// @pre mesa != NULL
bool es_mesa_valida(juego_t      *juego, 
                    const mesa_t *mesa,
                    bool         considerar_mozo,
                    bool         considerar_cocina);

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
                              bool         considerar_cocina);

// Funciones TP 2
// TODO: documentar



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
///      [0, juego_t::cantidad_mesas)
pedido_t tomar_pedido(juego_t *juego, 
                      int     indice_mesa);

/// @brief Spawnea entidades en el campo de juego en base a un contador
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

#endif