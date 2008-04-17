#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int color;
} fruit;

typedef struct {
	fruit f;
	int size;
} orange;

int square (int x) {
	return x*x;
}

int squid (int x) {
	return x*2;
}

int main () {
	
	int (*hello)(int) = square;
	
	printf("%i\n", sizeof(square));
	

	
	return 0;
	
}

