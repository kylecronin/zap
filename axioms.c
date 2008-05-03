atom *add(acons *args, nspace *n) {
	if (!args)
		return newint(0);

	aint *car = cint(eval(args->car, n));
	aint *cdr = cint(add(ccons(args->cdr), n));
	
	if (car && cdr)
		return newint(car->i + cdr->i);
	else
		return NULL;
}

atom *quit(acons *args, nspace *n) {
	exit(0);
}

atom *mult(acons *args, nspace *n) {
	if (!args)
		return newint(1);

	aint *car = cint(eval(args->car, n));
	aint *cdr = cint(mult(ccons(args->cdr), n));
	
	if (car && cdr)
		return newint(car->i * cdr->i);
	else
		return NULL;
}

atom *lambda(acons *args, nspace *n) {
	if (!args)
	{
		printf("lambda: bad syntax\n");
		return NULL;
	}
	
	if (!args->car || *(args->car) == tsym || *(args->car) == tcons)
		if (args->cdr && *(args->cdr) == tcons)
			return newfun(args->car, ccons(args->cdr)->car, n);		

	printf("lambda: bad syntax\n");
	return NULL;
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
	
	nspace *copy = malloc(sizeof(nspace));
	*copy = *n;

	n->head = copy;
	n->name = name;
	n->link = what;
	return what;
	
}