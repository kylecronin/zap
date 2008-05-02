int eq(atom *a, atom *b) {
	if (*a != *b) return 0;
	
	switch (*a) {
		
		case tint:
			return ((aint *) a)->i == ((aint *) b)->i;
		case tchar:
			return ((achar *) a)->c == ((achar *) b)->c;
		case tsym:
			return !strcmp(((asym *) a)->s, ((asym *) b)->s);
		default:
			return a == b;
	}
}

atom *lookup(nspace *search, asym *name) {
	
	if (!search) return NULL;
	if (eq((atom *) name, (atom *) search->name))
		return search->link;
	return lookup(search->head, name);
}

atom* apply (afun*, atom*);
atom* eval (atom*, nspace*);


atom *apply (afun *fn, atom *args) {
	nspace *x = fn->n;
	if (*(fn->args) == tsym)
		x = define(x, (asym *) fn->args, (atom *) args);
	else
	{
		atom *fargs = fn->args, *aargs = args;
		while (fargs && *fargs == tcons && aargs)
		{
			x = define(x, (asym *) ((acons *) fargs)->car, ((acons *) aargs)->car);
			aargs = ((acons *) aargs)->cdr;
			fargs = ((acons *) fargs)->cdr;
		}
		if (fargs && *fargs == tcons)
			printf("too few arguments\n");
		else
			if (fargs)
				x = define(x, (asym *) fargs, aargs);
			else
				if (aargs)
					printf("too many arguments\n");
	}
	
	
	eval(fn->body, x);
	
	return (atom *) args;
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