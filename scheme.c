#include <stdio.h>
#include <stdlib.h>

#include "types.c"

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
	
	
	Of course, there is the bear of implementation difficulty hiding up ahead,
	and the bear of course is the continuation. How in the world do we
	implement a continuation? This is further compounded by my weak
	understanding about what a continuation is.
	
	The general rule of thumb is that the continuation is everpresent, but
	only appears when you call-with-current-continuation.


**/


atom *apply (afun *fn, atom *args) {
	
	
	
	
	return (atom *) args;
}

atom *eval (atom *expr) {
	return expr;
	
	if (!expr) return expr;
	if (*expr == tcons) {
		acons *c = (acons *) expr;
		if (c->car && *(c->car) == tfun)
			apply((afun *) c->car, c->cdr);
		
		
		
		return expr;
	}
	else
		return expr;
}



int main (int argc, const char * argv[]) {
	char buff[256];
	char *p;
	
	for (;;) {
		printf("> ");
		gets(buff);
		p = buff;
		print(eval(read(&p)));
		printf("\n");
	}
	
    return 0;
}