# TP1 Fundamentos de Programacion 
Este es un trabajo practico de C para la materia Fundamentos de Programacion. 
Para compilar y ejecutar la build de debugging hacer

> $ make \

Para compilar con el comando de la consigna
> $ make release

Para ejecutar el juego
> $ ./juego

Para compilar y ejecutar el test suite
> $ make tests \
> \$ ./test_suite

Para compilar de cero
> $ make clean

### Notas al corrector
Use algunas cosas no vistas en clase, si hace falta las puedo explicar. El 
programa se compone de 4 librerias: _restaurant_, _restaurant_utils_, _generador_ y 
_entrada_. Intente que _generador_ sea lo mas caja negra posible. La idea era esconder 
la implementacion de generador_t con un pointer opaco y que todo se haga mediante las 
funciones generador_*, pero como la estructura se crea en el stack tengo que exponer 
su definicion. En un intento de encapsular con documentacion agrege el sufijo _ 
a los miembros "privados". El formateo puede ser medio inconsistente por que lo 
hice a mano, no estoy familizarizado con ningun formatter de C. 