typedef struct {
	int color;
} fruit;

typedef struct {
	fruit f;
	int size;
} orange;

int main () {
	orange a;
	orange.f.color = 7;
	orange.size = 10;
	
	fruit *b = (fruit *) &a;
	
	printf("%i\n", b->color);
	
	return 0;
}