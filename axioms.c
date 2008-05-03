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

int lengthhelp(atom *p) {
	if (!p)
		return 0;
	return 1 + lengthhelp(ccons(p)->cdr);
}

atom *length(acons *args, nspace *n) {
	if (!listphelp(args->car))
		printf("length: invalid list\n");
	return newint(lengthhelp(eval(args->car, n)));
}


atom *numeq(acons *args, nspace *n) {
	if (!args)
		return t;
	
	
	
	
	
}

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

