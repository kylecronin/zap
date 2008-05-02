/**
	this typing system that I'm using is getting cumbersome. I think what
	I'm going to do is put ALL casting in overloaded helper functions.
	
	i.e. lookup(nspace*, asym*) will be called by lookup(nspace*, atom*)
	and the simple translation is done automatically.
	
	in fact, these functions can probably be declared inline, but I'll worry
	about those sort of distinctions later.
	
	
	haha, joke's on me - overloading is C++/Java and not C. This might be
	solveable by preprocessor macros, but for the time being, I'll just deal
	with it.
	
	the idea behind the casting is to make sure everything is in proper form.
	I might just switch it all to cast inside the operation itself. That would
	declutter the function calls.
	
	Another thing that annoys me is the whole cast-and-get thing. I think
	I'll define a new set of operations that perform the appropriate casting
	
	asym *csym(atom *x) {
		if (x && *x == tsym)
			return (asym *) x;
		printf("bad symbol cast\n");
		return NULL;
	}
	
	these can also have another purpose - checking nullallity ;)
	
	atom *y = [something];
	asym *x;
	if (x = csym(y)) {
		[do something with x]
	}
	else {
		[y was null/not of correct type]
	}
	
	my new project is rewriting all my existing code with this new
	functionality.

**/



int eq(atom *a, atom *b) {
	if (*a != *b) return 0;
	
	switch (*a) {
		
		case tint:
			return cint(a)->i == cint(b)->i;
		case tchar:
			return cchar(a)->c == cchar(b)->c;
		case tsym:
			return !strcmp(csym(a)->s, csym(b)->s);
		default:
			return a == b;
	}
}

atom *lookup(nspace *search, asym *name) {
	if (!search) return NULL;
	if (eq(catom(name), catom(search->name)))
		return search->link;
	return lookup(search->head, name);
}


atom* apply (afun*, atom*);
atom* eval (atom*, nspace*);


atom *apply (afun *fn, atom *args) {
	nspace *x = fn->n;
	if (*(fn->args) == tsym)
		x = define(x, csym(fn->args), catom(args));
	else
	{
		atom *fargs = fn->args;
		while (fargs && *fargs == tcons && args)
		{
			x = define(x, csym(ccons(fargs)->car), ccons(args)->car);
			args = ccons(args)->cdr;
			fargs = ccons(fargs)->cdr;
		}
		if (fargs && *fargs == tcons)
			printf("too few arguments\n");
		else
			if (fargs)
				x = define(x, csym(fargs), args);
			else
				if (args)
					printf("too many arguments\n");
	}
	
	return eval(fn->body, x);
	
	//return (atom *) args;
}

/**
	eval will act only as a macroexpander, function calling will be left
	up to the individual operation. how will this work?
**/
atom *eval (atom *expr, nspace *n) {
	if (!expr) return expr;
	
	acons *c;
	atom *o;
	
	switch (*expr) {
		case tcons:
			c = (acons *) expr;
			o = eval(c->car, n);
			if (o && *o == tfun)
				return apply((afun *) o, c->cdr);
			else
				printf("invalid procedure");
			break;
		case tsym:
			return lookup(n, (asym *) expr);
		default:
			return expr;
	}
}