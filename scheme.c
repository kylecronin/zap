#include <stdio.h>
#include <stdlib.h>

/***********************************************************

ok, now onto the parse operation

to do this, I will simply use the C stack and recursive calls
to the parse function. it will take in a string and return the
equivalent atom.

the trick is allowing a recursive parse to destructively modify
the string pointer in order to parse the string left to right

the way I solved this before was a pointer to a string (which
was a pointer to a char). This is probably the best way to do this

***********************************************************/

//typedef enum { cons, integer, function } type;

typedef enum {
	tcons, tint, tchar, tfun, tnull
} type;

// atom struct with t being the type (as an enum) and
// a union that contains either a cons (named cons - confusing)
// just made a change that the enums begin with t, so a cons is a tcons
typedef struct atom {
	//enum { tcons, tint, tchar, tfun, tnull } t;
	type t;
	union {
		struct { struct atom *car, *cdr; } cons;
		int i;
		char c;
	};
} atom;

atom *newatom(type x) {
	atom *ret = (atom*) malloc(sizeof(atom));
	ret->t = x;
	return ret;
}


// the cons operation, allocates mem and applies the cons
atom *cons (atom *car, atom *cdr) {
	atom *ret = newatom(tcons);
	ret->cons.car = car;
	ret->cons.cdr = cdr;
	return ret;
}

// converts the int to an atom
atom *conv (int val) {
	atom *ret = newatom(tint);
	ret->i = val;
	return ret;
}

// returns the null 
atom *null () {
	return newatom(tnull);
}


atom *parse (char **input) {
	//atom *ret = (atom*) malloc(sizeof(atom));
	while (**input == ' ') *input++;
	if (**input == '(') {
		// it's a list
		//ret->t = cons;
		*input += 1;
		atom *ret = newatom(tcons);
		printf("begin list read\n");
		ret->cons.car = parse(input);
		printf("car read, reading cdr\n");
		ret->cons.cdr = null();
		printf("end list read\n");
		
		return ret;
	}
	else {
		//ret->t = integer;
		atom *ret = newatom(tint);
		ret->i = atoi(*input);
		printf("%i\n", *(*input+1));
		while (**input <= 57 && **input >= 48) *input++;
		printf("YEAHH! \"%s\"\n", *input);
	}
}

void print (atom *x) {
	if (x->t == tcons) {
		printf("(");
		print (x->cons.car);
		printf (" . ");
		print (x->cons.cdr);
		printf(")");
	}
	else {
		if (x->t == tint)
			printf("%i", x->i);
	}
}


int main (int argc, const char * argv[]) {
	/*char readb[256];
	
	char *z = "(1 2 3)";
	
	atom *x = parse(&z);
	print(x);

    printf("arc> ");
	//gets(readb);*/
	
	atom *x = cons(conv(2), conv(3));
	print(x);
	
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
 * since this is SUPPOSED to be Arc, I'll call it null
**/