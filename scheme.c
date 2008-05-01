#include <stdio.h>
#include <stdlib.h>

#include "types.c"

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
		//printf("o");
		switch (*x) {
			case tint: printf("%i", ((aint *) x)->i); break;
			case tchar: printf("#\\%c", ((achar *) x)->c); break;
			case tstring: printf("\"%s\"", ((astring *) x)->s); break;
			case tsym: printf("%s", ((asym *) x)->s); break;
		}
	}
}


atom *eval (atom *expr) {
	if (!expr) return expr;
	if (*expr == tcons) {
		
		return expr;
	}
	else
		return expr;
}



int main (int argc, const char * argv[]) {
	char buff[256];
	char *p;
	
	for (;;) {
		printf("> ");
		gets(buff);
		p = buff;
		print(eval(read(&p)));
		printf("\n");
	}
	
    return 0;
}