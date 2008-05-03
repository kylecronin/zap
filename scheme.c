#include <stdio.h>
#include <stdlib.h>

#include "types.c"
#include "operators.c"
#include "axioms.c"

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
	
	OK, more on the namespace - there is a small problem I'm having with
	reconciling the current linked list namespace and the definition of
	mutually recursive functions. There are basically two ways of handling
	this: the first is to do a rewrite of the namespace, which is something
	I don't want to do, to be something more amenable to change. The second
	is something that I initially rejected but am growing more fond of, and
	that is overwriting the nspace object with a new nspace object.
	
	
	OOH, better still, I can just postpend (instead of prepending). Well, this
	might not work for using define to redefine an object. I tend to like the
	idea that the most recent should be at the top. Prepending it is.
	
	
	I also need to consider how to handle "axiomatic" operations. I have a
	feeling that I will want to define all user reachable operations in
	"software" and have them call the "hardware" operations. How do I do this?
	
	I think I'll just define an axiomatic type, tax, and just have a switch
	inside eval.

**/


int main (int argc, const char * argv[]) {
	char buff[256];
	char *p;
	nspace *n = NULL;
	n = define(n, (asym *) newsym("x"), newint(3));
	
	atom *fn = newfun(newcons(newsym("x"), NULL), newsym("x"), n);
	n = define(n, (asym *) newsym("identity"), fn);
	n = define(n, csym(newsym("+")), newax(add));
	n = define(n, csym(newsym("-")), newax(sub));
	n = define(n, csym(newsym("*")), newax(mult));
	n = define(n, csym(newsym("exit")), newax(quit));
	n = define(n, csym(newsym("lambda")), newax(lambda));
	n = define(n, csym(newsym("define")), newax(def));
	n = define(n, csym(newsym("length")), newax(length));
	n = define(n, csym(newsym("quote")), newax(quote));
	n = define(n, csym(newsym("list?")), newax(listp));
	n = define(n, csym(newsym("car")), newax(car));
	n = define(n, csym(newsym("cdr")), newax(cdr));
	n = define(n, csym(newsym("=")), newax(numeq));
	n = define(n, csym(newsym("cons")), newax(cons));
	n = define(n, csym(newsym("if")), newax(aif));
	n = define(n, csym(newsym("null?")), newax(nullp));
	n = define(n, csym(newsym("let")), newax(let));
	n = define(n, csym(newsym("let*")), newax(lets));
	
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