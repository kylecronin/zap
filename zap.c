#include <stdio.h>
#include <stdlib.h>

char *bos;
int debug;

//#include "types.c"
#include "parse.c"
#include "operators.c"
#include "axioms.c"

/**

	All right, now that I have some basic procedures down, I think that I
	want to take the time to reorganize how they are defined and loaded. As
	it currently stands I manually create a namespace in the main method.
	
	

**/

					// why is this const?
int main (int argc, const char * argv[]) {
	char bosv;
	bos = &bosv;
	//printf("bos: %i\n", bos);
	
	if (argc > 1 && !strcmp(argv[1], "-d"))
		debug = 1;
	
	char buff[8192];
	char *p;
	nspace *n = NULL;
	//n = define(n, (asym *) newsym("x"), newint(3));
	
	atom *fn = newfun(newcons(newsym("x"), NULL),
				ccons(newcons(newsym("x"), NULL)), n);
	n = define(n, (asym *) newsym("identity"), fn);
	n = define(n, csym(newsym("+")), newax(add));
	n = define(n, csym(newsym("-")), newax(sub));
	n = define(n, csym(newsym("*")), newax(mult));
	n = define(n, csym(newsym("exit")), newax(quit));
	n = define(n, csym(newsym("lambda")), newax(lambda));
	n = define(n, csym(newsym("define")), newax(def));
//	n = define(n, csym(newsym("length")), newax(length));
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
	n = define(n, csym(newsym("set!")), newax(set));
	n = define(n, csym(newsym("letrec")), newax(letrec));
	n = define(n, csym(newsym("apply")), newax(sapply));
	n = define(n, csym(newsym("or")), newax(or));
	n = define(n, csym(newsym("begin")), newax(begin));
	n = define(n, csym(newsym("equal?")), newax(equal));
	n = define(n, csym(newsym("print")), newax(sprint));
	n = define(n, csym(newsym("defmacro")), newax(defmacro));
	n = define(n, csym(newsym("call/cc")), newax(callcc));
	n = define(n, csym(newsym("<")), newax(lt));
	n = define(n, csym(newsym("quasiquote")), newax(quasiquote));
	n = define(n, csym(newsym("load")), newax(load));
	n = define(n, csym(newsym("not")), newax(not));
	n = define(n, csym(newsym("and")), newax(and));
	n = define(n, csym(newsym("eq?")), newax(realeq));
	
	printf("zap -- western flyer\n");
	
	loadfile("bootstrap.scm", n);
	
	for (;;) {
		printf("> ");
		//gets(buff);
		//p = buff;
		print(eval(read(stdin), n));
		//print(read(&p));
		printf("\n");
	}
	
    return 0;
}