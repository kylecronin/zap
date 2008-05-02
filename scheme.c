#include <stdio.h>
#include <stdlib.h>

#include "types.c"
#include "operators.c"

/**

	Now that I have reading/printing down, I need to really figure out what
	I want to do with regard to functions. First I'll need a namespace, which
	abstractly is simply a lookup table that associates a name with a
	corresponding atom. However, lexical scoping can mess this up. The simple
	solution is to do the namespace lookup immediately when the function
	is defined. This has problems when using set! because that updates the
	lexical namespace.
	
	What if we were to have a tree-like namespace structure? This would allow
	us to keep the functions intact as they are defined. This would even allow
	us to create a new namespace for each time the function is called, and
	allow the expression to simply evaluate itself.

**/


int main (int argc, const char * argv[]) {
	char buff[256];
	char *p;
	nspace *n = NULL;
	n = define(n, (asym *) newsym("x"), newint(3));
	
	atom *fn = newfun(newcons(newsym("x"), NULL), newsym("x"), n);
	n = define(n, (asym *) newsym("identity"), fn);
	
	for (;;) {
		printf("> ");
		gets(buff);
		p = buff;
		print(eval(read(&p), n));
		//print(read(&p));
		printf("\n");
	}
	
    return 0;
}