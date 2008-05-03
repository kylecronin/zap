atom *add(atom *args, nspace *n) {
	if (!args)
		return newint(0);
	if (*args != tcons)
	{
		printf("+: argument list makes no sense!\n");
		return NULL;
	}
	
	acons *al = ccons(args);
	aint *car = cint(eval(al->car, n));
	aint *cdr = cint(add(al->cdr, n));
	
	if (car && cdr)
		return newint(car->i + cdr->i);
	else
		return NULL;
}

atom *quit(atom *args, nspace *n) {
	exit(0);
}

atom *mult(atom *args, nspace *n) {
	if (!args)
		return newint(1);
	if (*args != tcons)
	{
		printf("*: argument list makes no sense!\n");
		return NULL;
	}
	
	acons *al = ccons(args);
	aint *car = cint(eval(al->car, n));
	aint *cdr = cint(mult(al->cdr, n));
	
	if (car && cdr)
		return newint(car->i * cdr->i);
	else
		return NULL;
}

atom *lambda(atom *args, nspace *n) {
	if (!args || *args != tcons)
	{
		printf("lambda: bad syntax\n");
		return NULL;
	}
	acons *c = ccons(args);
	if (!c->car || *(c->car) == tsym || *(c->car) == tcons)
		if (c->cdr && *(c->cdr) == tcons)
			return newfun(c->car, ccons(c->cdr)->car, n);		

	printf("lambda: bad syntax\n");
	return NULL;
}