.SECONDEXPANSION:
BUILD?=debug
SANITIZER?=address

TEST_DIR=tests
LIBS=generador restaurant_utils restaurant entrada vector_pedidos vector_operaciones

cflags.release= -Wall -Werror -Wconversion -std=c99
cflags.debug= -I./ ${cflags.release} -fPIE -O0 -fanalyzer -g3 -fsanitize=${SANITIZER} -fsanitize=undefined -Wextra -Wundef -Wcast-align -Wstrict-overflow=5 -Wswitch-enum -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wno-unused-parameter


CFLAGS=${cflags.${BUILD}}
LDFLAGS=${ldflags.${BUILD}}

all: juego.o ${addsuffix .o,${LIBS}}
	${CC} ${CPPFLAGS} ${CFLAGS} ${LDFLAGS} ${LDLIBS} $^ -o juego

tests: ${TEST_DIR}/restaurant_utils.tests.o ${addsuffix .o,${LIBS}}
	${CC} ${CPPFLAGS} ${CFLAGS} ${LDFLAGS} ${LDLIBS} $^ -o test_suite

%.tests.o: %.tests.c
	${CC} ${CPPFLAGS} ${CFLAGS} ${LDFLAGS} ${LDLIBS} -c $< -o $@

release:
	gcc *.c -o juego -std=c99 -Wall -Wconversion -Werror -lm	

clean:
	clear
	rm *.o ${TEST_DIR}/*.o juego test_suite