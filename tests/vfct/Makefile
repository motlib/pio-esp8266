# This is a temporary quick hack to build this one test

INCLUDE_DIRS=-I../unity/src -I../../src

all:
	gcc ${INCLUDE_DIRS} test_vfct.cpp ../unity/src/unity.c -o test_vfct

run: all
	./test_vfct
