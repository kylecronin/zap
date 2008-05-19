/* added test */
atom *begin(acons *args, nspace *n) {
	if (args)
	{
		atom *val = eval(args->car, n);
		if (args->cdr)
			return begin(ccons(args->cdr), n);
		return val;
	}
	return NULL;
}


atom *quit(acons *args, nspace *n) {
	exit(0);
}

atom *lambda(acons *args, nspace *n) {
	if (!args)
	{
		printf("lambda: bad syntax\n");
		return NULL;
	}
	
	if (!args->car || *(args->car) == tsym || *(args->car) == tcons)
		if (args->cdr && *(args->cdr) == tcons)
			return newfun(args->car, ccons(args->cdr), n);		

	printf("lambda: bad syntax\n");
	return NULL;
}

atom *defhelp(asym *name, atom *what, nspace *n) {
	nspace *copy = malloc(sizeof(nspace));
	*copy = *n;

	n->head = copy;
	n->name = name;
	n->link = what;
	return what;
}

atom *def(acons *args, nspace *n) {
	if (!args)
	{
		printf("define: bad syntax\n");
		return NULL;
	}
	atom *what;
	asym *name;
	
	switch (*(args->car))
	{
		case tsym:
			name = csym(args->car);
			what = eval(ccons(args->cdr)->car, n);
			break;
		case tcons:
			name = csym(ccons(args->car)->car);
			what = lambda(ccons(newcons(ccons(args->car)->cdr, args->cdr)), n);
			break;
	}
	
	return defhelp(name, what, n);
}

atom *defmacro(acons *args, nspace *n) {
	asym *name = csym(ccons(args->car)->car);
	atom *what = lambda(ccons(newcons(ccons(args->car)->cdr, args->cdr)), n);
	*what = tmac;
	return defhelp(name, what, n);
}

atom *quote(acons *args, nspace *n) {
	return args->car;
}

atom *car(acons *args, nspace *n) {
	atom *t = eval(args->car, n);
	if (*t == tcons)
		return ccons(t)->car;
	printf("car: not a pair\n");
	return NULL;
}

atom *cdr(acons *args, nspace *n) {
	atom *t = eval(args->car, n);
	if (*t == tcons)
		return ccons(t)->cdr;
	printf("cdr: not a pair\n");
	return NULL;
}

int listphelp(atom *p) {
	if (!p)
		return 1;
	return (*p == tcons) && listphelp(ccons(p)->cdr);
}

atom *listp(acons *args, nspace *n) {
	if (!args)
	{
		printf("list?: expects 1 argument\n");
		return NULL;
	}
	return newbool(listphelp(eval(args->car, n)));
}

int lengthhelp(acons *p) {
	if (!p)
		return 0;
	return 1 + lengthhelp(ccons(p->cdr));
}

atom *length(acons *args, nspace *n) {
	atom *what = eval(args->car, n);
	if (!listphelp(what))
	{
		printf("length: invalid list\n");
		return NULL;
	}
	return newint(lengthhelp(ccons(what)));
}

atom *cons(acons *args, nspace *n) {
	if (!args || lengthhelp(args) != 2)
	{
		printf("cons: expects 2 arguments\n");
		return NULL;
	}
	atom *car = eval(args->car, n), *cdr = eval(ccons(args->cdr)->car, n);
	return newcons(car, cdr);
}

atom *aif(acons *args, nspace *n) {
	if (!args || lengthhelp(args) != 3)
	{
		printf("if: expects 3 arguments\n");
		return NULL;
	}
	if(eval(args->car, n) != f)
		return eval(ccons(args->cdr)->car, n);
	else
		return eval(ccons(ccons(args->cdr)->cdr)->car, n);
}

atom *numeq(acons *args, nspace *n) {
	if (!args)
		return t;
	if (lengthhelp(args) >= 2)
	{

		aint 	*a = cint(eval(args->car, n)),
				*b = cint(eval(ccons(args->cdr)->car, n));
		if (a && b)
			if (lengthhelp(args) == 2)
				return newbool(a->i == b->i);
			else
				return newbool(a->i == b->i && numeq(ccons(args->cdr), n) == t);
		else
		{
			printf("=: not a number\n");
			return NULL;
		}
	}
	else
	{
		printf("=: expects at least 2 arguments\n");
		return NULL;
	}
}

atom *nullp(acons *args, nspace *n) {
	if (lengthhelp(args) == 1)
	{
		atom *what = eval(args->car, n);
		return what ? f : t;
	}
	printf("null?: expects 1 argument\n");
	return NULL;
}

atom *add(acons *args, nspace *n) {
	if (!args)
		return newint(0);

	aint *car = cint(eval(args->car, n));
	aint *cdr = cint(add(ccons(args->cdr), n));
	
	if (car && cdr)
		return newint(car->i + cdr->i);
	else
	{
		printf("+: bad int\n");
		return NULL;
	}
}

atom *sub(acons *args, nspace *n) {
	if (!args)
	{
		printf("-: expects at least 1 argument\n");
		return NULL;
	}
	int len = lengthhelp(args);
	aint *car = cint(eval(args->car, n));
	
	if (car)
		if (len == 1)
			return newint(0 - car->i);
		else
			return newint(car->i - cint(add(ccons(args->cdr), n))->i);
	printf("-: not a number\n");
	return NULL;
}

atom *mult(acons *args, nspace *n) {
	if (!args)
		return newint(1);

	aint *car = cint(eval(args->car, n));
	aint *cdr = cint(mult(ccons(args->cdr), n));
	
	if (car && cdr)
		return newint(car->i * cdr->i);
	else
	{
		printf("*: bad int\n");
		return NULL;
	}
}

int sethelp(nspace *search, asym *name, atom *link) {
	if (!search) return 0;
	if (eq(catom(name), catom(search->name)))
	{
		search->link = link;
		return 1;
	}
	return sethelp(search->head, name, link);
}

atom *set(acons *args, nspace *n) {
	if (!n)
	{
		printf("set!: unbound symbol\n");
		return NULL;
	}
	if (lengthhelp(args) != 2)
	{
		printf("set!: expects 2 arguments\n");
		return NULL;
	}
	
	asym *name = csym(args->car);
	atom *link = eval(ccons(args->cdr)->car, n);
	
	if (sethelp(n, name, link))
		return link;
	printf("set!: undefined symbol\n");
	return NULL;
}

atom *let(acons *args, nspace *n) {
	if (lengthhelp(args) < 2) {
		printf("let: expects at least 2 arguments\n");
		return NULL;
	}
	if (!listphelp(args->car))
	{
		printf("let: expects list as first argument\n");
		return NULL;
	}
	
	nspace *new = n;
	acons *bl = ccons(args->car), *c;
	
	while (bl)
		if ((c = ccons(bl->car)) && *(c->car) == tsym && lengthhelp(c) == 2)
		{
			// legit list
			new = define(new, csym(c->car), eval(ccons(c->cdr)->car, n));
			bl = ccons(bl->cdr);
		}
		else
			printf("let: invalid let pair\n");
	
	return begin(ccons(args->cdr), new);
}

atom *lets(acons *args, nspace *n) {
	if (lengthhelp(args) < 2) {
		printf("let*: expects at least 2 arguments\n");
		return NULL;
	}
	if (!listphelp(args->car))
	{
		printf("let*: expects list as first argument\n");
		return NULL;
	}
	
	nspace *new = n;
	acons *bl = ccons(args->car), *c;
	
	while (bl)
		if ((c = ccons(bl->car)) && *(c->car) == tsym && lengthhelp(c) == 2)
		{
			// legit list
			new = define(new, csym(c->car), eval(ccons(c->cdr)->car, new));
			bl = ccons(bl->cdr);
		}
		else
			printf("let*: invalid let pair\n");
	
	return begin(ccons(args->cdr), new);
}

atom *letrec(acons *args, nspace *n) {
	if (lengthhelp(args) < 2) {
		printf("letrec: expects at least 2 arguments\n");
		return NULL;
	}
	if (!listphelp(args->car))
	{
		printf("letrec: expects list as first argument\n");
		return NULL;
	}
	
	acons *bl = ccons(args->car), *c;
	
	while (bl)
		if ((c = ccons(bl->car)) && *(c->car) == tsym && lengthhelp(c) == 2)
		{
			// legit list
			n = define(n, csym(c->car), NULL);
			bl = ccons(bl->cdr);
		}
		else
		{
			printf("let*: invalid let pair\n");
			return NULL;
		}
	
	bl = ccons(args->car);
	
	while (bl)
	{
		c = ccons(bl->car);
		sethelp(n, csym(c->car), eval(ccons(c->cdr)->car, n));
		bl = ccons(bl->cdr);
	}
	
	return begin(ccons(args->cdr), n);
}

atom *sapply(acons *args, nspace *n) {
	
	afun *fn = cfun(eval(args->car, n));
	acons *fa = ccons(eval(ccons(args->cdr)->car, n));
	
	return apply(fn, fa, n);
	
	
	// return eval(newcons(eval(args->car, n), eval(ccons(args->cdr)->car, n)), n);
	
	//return eval(catom(args), n);
}

atom *or(acons *args, nspace *n) {
	if (!args)
		return f;
	atom *val = eval(args->car, n);
	if (val == f)
		return or(ccons(args->cdr), n);
	return val;
}



int equalhelp(atom *a, atom *b) {
	/*printf("eh a=");
	print(a);
	printf(", b=");
	print(b);
	printf("\n");*/
	
	if (a && b && *a == *b && *a == tcons) {		
		return	equalhelp(ccons(a)->car, ccons(b)->car) &&
				equalhelp(ccons(a)->cdr, ccons(b)->cdr);
	}
	return eq(a, b);
}


atom *equal(acons *args, nspace *n) {
	if (lengthhelp(args) != 2) {
		printf("equal?: expected 2 arguments\n");
		return NULL;
	}
	
	return newbool(equalhelp(	eval(args->car, n),
								eval(ccons(args->cdr)->car, n)));
}

atom *sprint(acons *args, nspace *n) {
	if (args)
	{
		print(eval(args->car, n));
		return sprint(ccons(args->cdr), n);
	}
	return NULL;
}

atom *callcc(acons *args, nspace *n) {
	if (lengthhelp(args) != 1) {
		printf("call/cc: expected 1 argument\n");
		return NULL;
	}
	
	atom *arg = eval(args->car, n);
	
	if (*arg != tfun) {
		printf("call/cc: argument must be a function\n");
		return NULL;
	}
	
	acont *c = newcont();
	setjmp(c->c);
	printf("we jumped\n");
	
	if (c->s)
	{
		printf("returning from slumber\n");
		return c->ret;
	}
	else
	{
		c->s = 1;
		printf("initial return\n");
		return apply(cfun(arg), ccons(newcons(catom(c), NULL)), n);
	}
}

atom *le(acons *args, nspace *n) {
	aint *a = cint(eval(args->car, n));
	aint *b = cint(eval(ccons(args->cdr)->car, n));
	return newbool(a->i <= b->i);
}



















