#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef enum {
	tcons, tint, tchar, tfun, tstring, tsym, tbool
} atom;


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


typedef struct afun {
	atom t;// = tfun;
	nspace *n;
	acons *args, *body;
} afun;

atom *newfun(acons *args, acons *body, nspace *n) {
	afun *ret = malloc(sizeof(afun));
	ret->args = args;
	ret->body = body;
	ret->n = n;
	return (atom *) ret;
}


atom* readlist (char**);
atom* read (char**);

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
			ret = (atom *) newstring(*input + 1);
			*input = end + 1;
			break;
		case '#':
			if (**ip(input) == '\\')
				ret = (atom *) newchar(**ip(input));
			else
				printf("invalid # syntax\n");
			ip(input);
			break;
		default:
			if (**input >= 48 && **input <=57)
			{
				ret = (atom *) newint(atoi(*input));
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
				ret = (atom *) newsym(*input);
				*end = oldend;
				*input = end;
			}
			break;
			
	}
	
	return ret;
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
			case tint: printf("%i", ((aint *) x)->i); break;
			case tchar: printf("#\\%c", ((achar *) x)->c); break;
			case tstring: printf("\"%s\"", ((astring *) x)->s); break;
			case tsym: printf("%s", ((asym *) x)->s); break;
		}
	}
}

