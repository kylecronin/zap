#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int color;
} fruit;

typedef struct {
	fruit f;
	int size;
} orange;

int main () {
	orange a;
	a.f.color = 7;
	a.size = 10;
	
	fruit *b = (fruit *) &a;
	
	printf("%i\n", b->color);
	
	return 0;
}