#include <stdio.h>
#include <stdlib.h>

/*
**********************************************************

well it's been several days since I last worked on this stuff. in that time I have learned some more interesting things about C structs - enough to make me want to try this again. basically, if you have a pointer to a struct, you can convert that pointer seamlessly to one to the first item in the struct. this has the advantage of allowing an object-like hierarchy.

each type will have its own typedef. the atom typedef will be the supertype that will be used to extend all the other types.

Since the only data I want to store is the type of object it really is I think I will hybridize the type enum and the atom struct.

**********************************************************
*/

#include "types.c"



char **cw (char **p)
{
	while (**p == ' ') (*p)++;
	return p;
}

void ip (char **p) { (*p)++; }
void dp (char **p) { (*p)--; }



atom *parse (char **input)
{	
	// if the first character is '(', we're reading a list
	if (**cw(input) == '(')
	{
		// increment pointer, consume whitespace
		ip(input);
		
		// if we encounter ')', we've read a null
		if (**cw(input) == ')')
		{
			ip(input);
			return NULL;
		}
		
		atom *car, *cdr;
		
		car = parse(input);
		
		// if we encounter ')', we've read a list with 1 element
		if (**cw(input) == ')')
			cdr = NULL;
			
		// otherwise we'ver read in a pair or a list
		else
		{
			if (**input == '.')
			{
				ip(input);
				cdr = parse(input);
				if (**cw(input) == ')')
					ip(input);
				else
					printf("parse error: invalid dotted pair\n");
			}
			else
			{
				dp(input);
				**input = '(';
				cdr = parse(input);
				if (**cw(input) == ')')
					ip(input);
				else
					printf("parse error: invalid list [ read '%s' ]\n", *input);
			}	
		}	
		return newcons(car, cdr);
	}
	else
	{
		//ret->t = integer;
		atom *ret = newint(atoi(*input));
		while (**input >= 48 && **input <= 57)
			ip(input);
		return ret;
	}
}

/*

OK, we're sort of in a pickle here because of the list. The
easiest thing to do is to print it recursively, but this would
result in unwanted parenthesis. I guess the best way to do this
would be to create a loop instead

- if atom, print atom
- if list, print open paren '(' and enter list printing loop:
	- print car
	- is cdr null?
		- yes: end loop
		- no: is cdr cons?
			- no: print dot '.', print cdr, end loop
			- yes: set head to cdr, continue loop
- print close paren ')'

*/

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
		}
	}
}




int main (int argc, const char * argv[])
{
	char buff[256];
	char *p;
	
	print(newint(4));
	
	for (;;)
	{
		printf("> ");
		gets(buff);
		p = buff;
		atom *x = parse(&p);
		printf("parsed\n");
		print(x);
		printf("\n");
	}
	
    return 0;
}

