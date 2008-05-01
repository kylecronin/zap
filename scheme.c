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

/**
	I finally know what my problem is - I'm trying to read the close
	paren off a list that I've created out of thin air.
	
	This practice of modifying the list is dangerous, but there might be a
	way to salvage this
	
	If I insert a paren, I don't expect one at the end.
	
	
	
	new problem: either the read or the print of (1 (2) 3) is (1 (2))
	
	
	just tested the print, it works fine. so it is, as I suspected, the read
	operation.
	
	we need to step through this:
	parse (1 (2) 3)
		read (
		parse 1
		parse ((2) 3)
			read (
			parse (2)
			
			
	I, uh, fixed it. just needed an ip(input);
**/

atom *parse (char **input)
{	
	if (**cw(input) == '(')
	{
		ip(input);
		
		if (**cw(input) == ')')
		{
			ip(input);
			return NULL;
		}
		
		atom *car = parse(input), *cdr;
		
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
				/*printf("checking for ) at %i\n", *input - start);
				if (**cw(input) == ')')
					ip(input);
				else
					printf("parse error: invalid list [ read '%i' ]\n", *input - start);*/
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
	
	for (;;)
	{
		printf("> ");
		gets(buff);
		p = buff;
		print(parse(&p));
		printf("\n");
	}
	
    return 0;
}

