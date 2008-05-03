#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define catom(x) ((atom *) x)





typedef enum {
	tcons, tint, tchar, tfun, tstring, tsym, tbool, tax
} atom;

atom* readlist (char**);
atom* read (char**);
void print (atom*);


atom sect = tbool, secf = tbool;
atom *t = &sect, *f = &secf;

atom *newbool(int val) {
	return val ? t : f;
}




typedef struct acons {
	atom t, *car, *cdr;
} acons;

atom *newcons(atom *car, atom *cdr) {
	acons *ret = malloc(sizeof(acons));
	ret->t = tcons;
	ret->car = car;
	ret->cdr = cdr;
	return (atom *) ret;
}

acons *ccons(atom *x) {
	if (x && *x == tcons)
		return (acons *) x;
	// I've decided that null is a legit cons cast
	if (x)
		printf("bad cons cast\n"); 
	return NULL;
}


typedef struct aint {
	atom t;
	int i;
} aint;

atom *newint(int i) {
	aint *ret = malloc(sizeof(aint));
	ret->t = tint;
	ret->i = i;
	return (atom *) ret;
}

aint *cint(atom *x) {
	if (x && *x == tint)
		return (aint *) x;
	printf("bad int cast\n");
	return NULL;
}


typedef struct achar {
	atom t;
	char c;
} achar;

atom *newchar(char c) {
	achar *ret = malloc(sizeof(achar));
	ret->t = tchar;
	ret->c = c;
	return (atom *) ret;
}

achar *cchar(atom *x) {
	if (x && *x == tchar)
		return (achar *) x;
	printf("bad char cast\n");
	return NULL;
}


typedef struct astring {
	atom t;
	char *s;
} astring;

atom *newstring(char *s) {
	char *str = malloc(strlen(s));
	astring *ret = malloc(sizeof(astring));
	ret->t = tstring;
	ret->s = strcpy(str, s);
	return (atom *) ret;
}

astring *cstring(atom *x) {
	if (x && *x == tstring)
		return (astring *) x;
	printf("bad string cast\n");
	return NULL;
}


typedef struct asym {
	atom t;
	char *s;
} asym;

atom *newsym(char *s) {
	char *str = malloc(strlen(s));
	astring *ret = malloc(sizeof(asym));
	ret->t = tsym;
	ret->s = strcpy(str, s);
	return (atom *) ret;
}

asym *csym(atom *x) {
	if (x && *x == tsym)
		return (asym *) x;
	printf("bad sym cast\n");
	return NULL;
}


typedef struct nspace {
	struct nspace *head;
	asym *name;
	atom *link;
} nspace;
 
nspace *define(nspace *head, asym *name, atom *link) {
	nspace *ret = malloc(sizeof(nspace));
	ret->head = head;
	ret->name = name;
	ret->link = link;
	return ret;	
}

typedef struct aax {
	atom t;
	atom *(*a)(acons*, nspace*);
} aax;

atom *newax(atom *(*a)(acons*, nspace*)) {
	aax *ret = malloc(sizeof(aax));
	ret->t = tax;
	ret->a = a;
	return (atom *) ret;
}

aax *cax(atom *x)
{
	if (x && *x == tax)
		return (aax *) x;
	return NULL;
}



typedef struct afun {
	atom t;// = tfun;
	nspace *n;
	atom *args, *body;
} afun;

atom *newfun(atom *args, atom *body, nspace *n) {
	afun *ret = malloc(sizeof(afun));
	ret->t = tfun;
	ret->args = args;
	ret->body = body;
	ret->n = n;
	return (atom *) ret;
}

afun *cfun(atom *x) {
	if (x && *x == tfun)
		return (afun *) x;
	printf("bad fun cast\n");
	return NULL;
}




char **cw (char **p) { while (**p == ' ') (*p)++; return p; }
char **ip (char **p) { (*p)++; return p; }

atom *readlist (char **input) {
	atom *ret, *car, *cdr;
	
	switch (**cw(input)) {
		case ')':
			ret = NULL;
			ip(input);
			break;
		case '.':
			ret = read(ip(input));
			if (**cw(input) == ')')
				ip(input);
			else
				printf("invalid dotted list\n");
			break;
		default:
			car = read(input);
			cdr = readlist(input);
			return newcons(car, cdr);
	}
	
	return ret;
}

atom *read (char **input) {	
	atom *ret;
	char *end, oldend;
	
	switch(**cw(input)) {
		case '(':
			return readlist(ip(input));
		case '"':
			end = *input + 1;
			while (*end != '"')
				end++;
			*end = '\0';
			ret = newstring(*input + 1);
			*input = end + 1;
			break;
		case '#':
			if (**ip(input) == '\\')
				ret = newchar(**ip(input));
			else
			{
				if (**input == 't')
					ret = newbool(1);
				else if (**input == 'f')
					ret = newbool(0);
				else
					printf("invalid # syntax\n");
			}
				
			ip(input);
			break;
		case '\'':
			ret = newcons(newsym("quote"), newcons(read(ip(input)), NULL));
			break;
		default:
			if (**input >= 48 && **input <=57)
			{
				ret = newint(atoi(*input));
				while (**input >= 48 && **input <= 57)
					ip(input);
			}
			else
			{
				end = *input + 1;
				while (*end && *end != ' ' && *end != '(' && *end !=')')
					end++;
				oldend = *end;
				*end = '\0';
				ret = newsym(*input);
				*end = oldend;
				*input = end;
			}
			break;
			
	}
	
	return ret;
}

void printns (nspace *n) {
	if (n)
	{
		print(catom(n->name));
		printf(" -> ");
		print(n->link);
		printf(", ");
		printns(n->head);
	}
	else
		printf("end ns");
}


void print (atom *x) {
	if (!x)
		printf("()");
	else if (*x == tcons) {
		acons *c = (acons *) x;
		printf("(");
		print(c->car);
		while (c->cdr && *(c->cdr) == tcons) {
			c = (acons *) c->cdr;
			printf(" ");
			print(c->car);
		}
		if (c->cdr) {
			printf(" . ");
			print(c->cdr);
		}
		printf(")");
	}
	else {
		switch (*x) {
			case tint: printf("%i", cint(x)->i); break;
			case tchar: printf("#\\%c", cchar(x)->c); break;
			case tstring: printf("\"%s\"", cstring(x)->s); break;
			case tsym: printf("%s", csym(x)->s); break;
			case tbool: printf(x == t ? "#t" : "#f"); break;
			case tfun:
				printf("#fun#");
				break;
				printf("args: [");
				print(cfun(x)->args);
				printf("] body: [");
				print(cfun(x)->body);
				printf("] ns: [");
				printns(cfun(x)->n);
				printf("]");
				break;
		}
	}
}
