atom *begin(acons*, nspace*);

atom *lookup(nspace *search, asym *name) {
	//printf("starting lookup of %s\n", name->s);
	if (!search)
	{
		printf("lookup of %s failed\n", name->s);
		exit(1);
		return NULL;
	}
	if (eq(catom(name), catom(search->name)))
		return search->link;
	return lookup(search->head, name);
}


atom* apply (afun*, acons*, nspace*);
atom* eval (atom*, nspace*);
atom* evallist (acons*, nspace*);

atom *contperm;

atom *icont(acont *c, atom *ret) {
	//printf("icont called\n");
	static int size;
	static char *from, *to;
	
	char tos;
	
	if (c->respt < &tos)
		return icont(c, ret);
	
	contperm = ret;
	size = c->size;
	from = c->stack;
	to = c->respt;
	
	while (size--)
		*(to++) = *(from++);

	longjmp(c->registers, 0);
}

atom *apply (afun *fn, acons *args, nspace *n) {	
	nspace *x = fn->n;
	if (args)
		if (*(fn->args) == tsym)
			x = define(x, csym(fn->args), evallist(args, n));
		else
		{
			atom *fargs = fn->args;
			while (fargs && *fargs == tcons && args)
			{
				x = define(x, csym(ccons(fargs)->car), eval(args->car, n));
				args = ccons(args->cdr);
				fargs = ccons(fargs)->cdr;
			}
			if (fargs && *fargs == tcons)
				printf("too few arguments\n");
			else
				if (fargs)
					x = define(x, csym(fargs), evallist(args, n));
				else
					if (args)
						printf("too many arguments\n");
		}
	
	if (*catom(fn) == tfun)
		return begin(fn->body, x);
	return begin(fn->body, n);
}

/**
	eval will act only as a macroexpander, function calling will be left
	up to the individual operation. how will this work?
**/
atom *eval (atom *expr, nspace *n) {
	char tos;
	//printf("eval tos: %i\n", &tos);
	
	if (debug) {
		printf("eval ["); print(expr); printf("]\n");
	}
	
	if (!expr) return expr;
	
	acons *c;
	atom *o;
	
	//printf("eval A\n");
	
	switch (*expr) {
		case tcons:
			//printf("eval B\n");
			c = (acons *) expr;
			o = eval(c->car, n);
			if (!o)
			{
				printf("invalid procedure: null\n");
				return NULL;
			}
		//	printf("eval C\n");
			atom *xyz;
			switch (*o)
			{
				case tfun: case tmac:
					//printf("eval D\n");
					return apply((afun *) o, ccons(c->cdr), n);
				case tax:
					//printf("eval E\n");
					return (cax(o)->a)(ccons(c->cdr), n);
				case tcont:
					//printf("eval F\n");
					if (!c->cdr)
					{
						printf("continuation requires an argument to return\n");
						exit(0);
					}
					xyz = eval(ccons(c->cdr)->car, n);
					//printf("eval J\n");
					return icont((acont *) o, xyz);
			}
			break;
		case tsym:
			//printf("eval G\n");
			return lookup(n, (asym *) expr);
		default:
			//printf("eval H\n");
			return expr;
	}
	//printf("eval I\n");
}

atom *evallist (acons *list, nspace *n) {
	if (!list)
		return NULL;
	return newcons(eval(list->car, n), evallist(ccons(list->cdr), n));
}

void loadfile(char *name, nspace *n) {
	
	FILE *file = fopen(name, "r");
	int c;
	
	for (;;) {
		while ((c = fgetc(file)) != EOF && isspace(c));
		if (c == EOF) break;
		ungetc(c, file);
		eval(read(file), n);
	}
	
	fclose(file);
}