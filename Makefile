all:
	gcc -g -o ntru *.c ./tests/*.c

#gcc -Wall -c arithmetic.c dpke.c encodings.c kat.c kem.c ntru.c sampling.c utils.c bitstrings.c
#cd tests && gcc -Wall -c utils.c encodings.c
#gcc -Wall -o ntru arithmetic.o dpke.o encodings.o kat.o kem.o ntru.o sampling.o utils.o bitstrings.c tests/utils.o tests/encodings.o -L . -l rng -Wl,-rpath,.
