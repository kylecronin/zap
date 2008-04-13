#include <stdio.h>
#include <stdlib.h>

/***********************************************************

it's been a while since I last coded in this file.


***********************************************************/

//typedef enum { cons, integer, function } type;

typedef struct test {
	int x;
	struct test *y;
} test;

typedef struct atom {
	enum { cons, integer, character, function, nil } t;
	union {
		struct { struct atom *car, *cdr; } cons;
		int i;
		char c;
	};
} atom;

atom *fcons (atom *car, atom *cdr) {
	atom *ret = (atom*) malloc(sizeof(atom));
	ret->t = cons;
	ret->cons.car = car;
	ret->cons.cdr = cdr;
	return ret;
}

atom *conv (int val) {
	atom *ret = (atom*) malloc(sizeof(atom));
	ret->t = integer;
	ret->i = val;
	return ret;
}

atom *fnil () {
	atom *ret = (atom*) malloc(sizeof(atom));
	ret->t = nil;
	return ret;
}

atom *parse (char **input) {
	atom *ret = (atom*) malloc(sizeof(atom));
	while (**input == ' ') *input++;
	if (**input == '(') {
		// it's a list
		ret->t = cons;
		*input += 1;
		ret->cons.car = parse(input);
		ret->cons.cdr = fnil();
		printf("YEAHH! \"%s\"\n", *input);
	}
	else {
		ret->t = integer;
		
		char *temp = *input;
		ret->i = atoi(*input);
		printf("%i\n", *(*input+1));
		while (**input <= 57 && **input >= 48) *input++;
		printf("YEAHH! \"%s\"\n", *input);
	}
	return ret;
}

void print (atom *x) {
	if (x->t == cons) {
		printf("(");
		print (x->cons.car);
		if (x->cons.cdr->t == cons || x->cons.cdr->t == nil)
			print (x->cons.cdr);
		else {
			printf (". ");
			print (x->cons.cdr);
		}
		printf(")");
	}
	else {
		if (x->t == integer)
			printf("%i ", x->i);
	}
}
	

int main (int argc, const char * argv[]) {
	char readb[256];
	
	char *z = "(1 2 3)";
	
	atom *x = parse(&z);
	print(x);

    printf("arc> ");
	//gets(readb);
	
    return 0;
}

/**
 * This is my super experimental version
 *
 * this is a git test
 *
 * hello there
 *
 * Gaah, it's tough to come up with a way to represent all that's required in memory
 * without resorting to some ugly hacks. Obviously every object needs a type section
 * and a data section, but it's tough to find a flexible way to implement the data
 * section. This would really be aided with a type hierarchy.
 *
 * I could create my own ad-hoc version, I guess. I would have to use some pointer
 * voodoo, but it could be done.
 *
 * So let it be written, so let it be done. I have something basic which might work.
 * I think I can even have the pointer point to a C int for the int, a C char for a
 * char, etc.
 *
 * Totally by accident I discovered the best way to do this - unions! This enables
 * me to define a union with all the possible data elements that I want!
 *
 * OK, so I have something that might work
 *
 * Now, for some basics in reading in/out the data
 * we'll use the C function stack with recursive calls
 * first, discern what sort of API will be needed
 * everything will be read in "inert" at first
 * perhaps later some sort of namespace translation can be added
 * ok, so since we're dealing with numbers as the only atoms, we
 * will test on an input like this:
 *
 * (1 (2 3 ((4 5) 6 () 7)))
 *
 * right off the bat I realize that I need a null, wait I don't
 * C provides a null pointer for me :-/
 * my first instict was right, I do need a null. This needs to be an atom.
 * since this is SUPPOSED to be Arc, I'll call it nil
**/