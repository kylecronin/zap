atom *add(atom *args, nspace *n) {
	if (!args)
		return newint(0);
	if (*args != tcons)
	{
		printf("add: argument list makes no sense!\n");
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