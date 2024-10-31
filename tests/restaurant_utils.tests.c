#include "restaurant_utils.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    assert(es_misma_coordenada((coordenada_t){0},(coordenada_t){0}) == true);
    assert(es_posicion_valida((coordenada_t){0}) == true);
    assert(es_posicion_valida((coordenada_t){.col=-1}) == false);
    assert(posicion_superpone_rectangulo((coordenada_t){
        .col = 2, .fil=2
    }, (rectangulo_t){
        .arriba_izq={.col=1, .fil=1},
        .abajo_der={.col=3, .fil=3}
    }) == true);
    assert(posicion_superpone_rectangulo((coordenada_t){
        .col = 2, .fil=1
    }, (rectangulo_t){
        .arriba_izq={.col=1, .fil=1},
        .abajo_der={.col=3, .fil=3}
    }) == true);
    assert(posicion_superpone_rectangulo((coordenada_t){
        .col = 1, .fil=1
    }, (rectangulo_t){
        .arriba_izq={.col=1, .fil=1},
        .abajo_der={.col=3, .fil=3}
    }) == true);
    assert(posicion_superpone_rectangulo((coordenada_t){
        .col = 16, .fil=13
    }, (rectangulo_t){
        .arriba_izq={.col=15, .fil=13},
        .abajo_der={.col=16, .fil=14}
    }) == true);
    assert(posicion_superpone_rectangulo((coordenada_t){
        .col = 4, .fil=4
    }, (rectangulo_t){
        .arriba_izq={.col=1, .fil=1},
        .abajo_der={.col=3, .fil=3}
    }) == false); 

    assert(posicion_superpone_mesa(
        &(juego_t){
            .mesas={[0]={.cantidad_lugares=1, .posicion={{.col=2,.fil=2}}}},
            .cantidad_mesas=1
        }, (coordenada_t){.col=2, .fil=2}, false
    ) == true);

    assert(posicion_superpone_mesa(
        &(juego_t){
            .mesas={[0]={.cantidad_lugares=1, .posicion={{.col=1,.fil=1}}}},
            .cantidad_mesas=1
        }, (coordenada_t){.col=2, .fil=2}, false
    ) == false);

    assert(posicion_superpone_mesa(
        &(juego_t){
            .mesas={[0]={.cantidad_lugares=1, .posicion={{.col=1,.fil=1}}}},
            .cantidad_mesas=1
        }, (coordenada_t){.col=2, .fil=2}, true
    ) == true);


    printf("All tests passed\n");
}