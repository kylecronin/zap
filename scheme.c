#include <stdio.h>
#include <stdlib.h>

#include "types.c"


char **cw (char **p)
{
	while (**p == ' ') (*p)++;
	return p;
}

void ip (char **p) { (*p)++; }
void dp (char **p) { (*p)--; }

atom *read (char **input)
{	
	if (**cw(input) == '(')
	{
		ip(input);
		
		if (**cw(input) == ')')
		{
			ip(input);
			return NULL;
		}
		
		atom *car = read(input), *cdr;
		
		if (**cw(input) == ')')
		{
			ip(input);
			return newcons(car, NULL);
		}
			
		else
		{
			if (**input == '.')
			{
				ip(input);
				cdr = read(input);
				if (**cw(input) == ')')
					ip(input);
				else
					printf("read error: invalid dotted pair\n");
			}
			else
			{
				dp(input);
				**input = '(';
				cdr = read(input);
			}	
		}	
		return newcons(car, cdr);
	}
	else
	{
		if (**input == '"')
		{
			char *end = *input + 1;
			while (*end != '"')
				end++;
			*end = '\0';
			atom *ret = newstring(*input + 1);
			*input = end + 1;
			return ret;
		}
		else
		{
			atom *ret = newint(atoi(*input));
			while (**input >= 48 && **input <= 57)
				ip(input);
			return ret;
		}
	}
}

void print (atom *x)
{
	if (!x)
	{
		printf("()");
	}
	else if (*x == tcons)
	{
		acons *c = (acons *) x;
		printf("(");
		print(c->car);
		while (c->cdr && *(c->cdr) == tcons)
		{
			c = (acons *) c->cdr;
			printf(" ");
			print(c->car);
		}
		if (c->cdr)
		{
			printf(" . ");
			print(c->cdr);
		}
		printf(")");
	}
	else
	{
		//printf("o");
		switch (*x)
		{
			case tint: printf("%i", ((aint *) x)->i); break;
			case tstring: printf("\"%s\"", ((astring *) x)->s); break;
		}
	}
}


atom *eval (atom *expr)
{
	if (*expr == tcons)
	{
		
		return expr;
	}
	else
		return expr;
}



int main (int argc, const char * argv[])
{
	char buff[256];
	char *p;
	
	for (;;)
	{
		printf("> ");
		gets(buff);
		p = buff;
		print(eval(read(&p)));
		printf("\n");
	}
	
    return 0;
}