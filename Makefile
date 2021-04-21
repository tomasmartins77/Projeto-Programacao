GCC=gcc
GCC_FLAGS=-Wall -std=c11 -O3


all: main

main: main.c
	$(GCC) $(GCC_FLAGS) -o main main.c

clean::
	rm main