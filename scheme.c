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
	
	
	Of course, there is the bear of implementation difficulty hiding up ahead,
	and the bear of course is the continuation. How in the world do we
	implement a continuation? This is further compounded by my weak
	understanding about what a continuation is.
	
	The general rule of thumb is that the continuation is everpresent, but
	only appears when you call-with-current-continuation. The continuation
	represents the "future of the computation", and is often compared to the
	runtime stack. How do we represent this?
	
	One thing that seems painfully obvious is that recursive calls to the
	built-in eval have to be tossed. Like the namespaces, continations and
	the program "stack" have evolved into a tree.
	
	It's been said that the Chicken Scheme->C compiler represents this with
	functions that never return. I'm not sure how that helps though.
	
	If we were to represent everything with a continuation, each call to eval
	would not return a value, but call the current continuation with that
	value.
	
	(define (fact x)
		(if (= x 0) 1
			(* x (fact (- x 1)))))
			
	(fact 5)
	
	(define (fact x cc)
		(if (= x 0) (cc 1)
			(cc (* x (fact (- x 1))))))
			
	There is the school of thought that I could just implement continuations
	in the language via macros, a la Paul Graham.
	
	Another idea: use the down-pass in the recursion to create the
	continuations, then ....
	
	
	I think I'd best just take the pragmatic approach and ignore continuations
	for the moment.


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