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



atom *apply (afun *fn, atom *args) {
	
	
	
	
	return (atom *) args;
}

/**
	eval will act only as a macroexpander, function calling will be left
	up to the individual operation. how will this work?
**/
atom *eval (atom *expr, nspace *n) {
	if (!expr) return expr;
	
	acons *c;
	
	switch (*expr) {
		case tcons:
			c = (acons *) expr;
			if (c->car && *(c->car) == tfun)
				return apply((afun *) eval(c->car, n), c->cdr);
			else
				printf("invalid procedure");
			break;
		case tsym:
			return lookup(n, (asym *) expr);
		default:
			return expr;
	}
}