#include <stdio.h>
#include <stdlib.h>

/*
**********************************************************

ok, now onto the parse operation

git test

it is 2008-04-16

to do this, I will simply use the C stack and recursive calls to the parse
function. it will take in a string and return the equivalent atom.

the trick is allowing a recursive parse to destructively modify the string
pointer in order to parse the string left to right

the way I solved this before was a pointer to a string (which was a pointer to
a char). This is probably the best way to do this

ls

OK, new parse problem

this one is about the parsing of a list. any other atom can be parsed with one
"pass", but it's the list that must be done recursively.

- read '(', know we're dealing with a list or a pair
- consume whitespace 
- check for ')' - if it is, it's empty list/null
- parse car
- consume whitespace
- check for ')' - if it is, it's a list of 1 element
- check for '.' - if it is, it's a dotted pair
- insert fake '(' right before pointer
- parse as a new list


**********************************************************
*/

//typedef enum { cons, integer, function } type;

typedef enum
{
	tcons, tint, tchar, tfun, tnull
} type;

// atom struct with t being the type (as an enum) and
// a union that contains either a cons (named cons - confusing)
// just made a change that the enums begin with t, so a cons is a tcons
typedef struct atom
{
	//enum { tcons, tint, tchar, tfun, tnull } t;
	type t;
	union {
		struct { struct atom *car, *cdr; } cons;
		int i;
		char c;
	};
} atom;

atom *newatom(type x)
{
	atom *ret = (atom*) malloc(sizeof(atom));
	ret->t = x;
	return ret;
}


// the cons operation, allocates mem and applies the cons
atom *cons (atom *car, atom *cdr)
{
	atom *ret = newatom(tcons);
	ret->cons.car = car;
	ret->cons.cdr = cdr;
	return ret;
}

// converts the int to an atom
atom *conv (int val)
{
	atom *ret = newatom(tint);
	ret->i = val;
	return ret;
}

// returns the null 
atom *null ()
{
	return newatom(tnull);
}

/*
- read '(', know we're dealing with a list or a pair
- consume whitespace 
- check for ')' - if it is, it's empty list/null
- parse car
- consume whitespace
- check for ')' - if it is, it's a list of 1 element
- check for '.' - if it is, it's a dotted pair
- insert fake '(' right before pointer
- parse as a new list

seeing the relative simplicity in an iterative solution to the print
I've decided to attempt to both fix and refactor this function with
that same iterative process of reading a list:

- read '(', know we're dealing with a list or a pair
- consume whitespace
- check for ')', if it is return null
- have two pointers, head and temp, head pointing to the head
- begin read loop
	- 
	- is next ')'?
		- yes: end loop
		


*/

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
			return newatom(tnull);
		}
		
		// we now know we're returning a cons object	
		atom *ret = newatom(tcons);
		
		// read in the car via parse
		ret->cons.car = parse(input);
		
		// if we encounter ')', we've read a list with 1 element
		if (**cw(input) == ')')
			ret->cons.cdr = null();
			
		// otherwise we'ver read in a pair or a list
		else
		{
			if (**input == '.')
			{
				ip(input);
				ret->cons.cdr = parse(input);
				if (**cw(input) == ')')
					ip(input);
				else
					printf("parse error: invalid dotted pair\n");
			}
			else
			{
				dp(input);
				**input = '(';
				ret->cons.cdr = parse(input);
				if (**cw(input) == ')')
					ip(input);
				else
					printf("parse error: invalid list [ read '%s' ]\n", *input);
			}	
		}	
		return ret;
	}
	else
	{
		//ret->t = integer;
		atom *ret = newatom(tint);
		ret->i = atoi(*input);
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
	if (x->t == tcons)
	{
		printf("(");
		print(x->cons.car);
		while (x->cons.cdr->t == tcons)
		{
			x = x->cons.cdr;
			printf(" ");
			print(x->cons.car);
		}
		if (x->cons.cdr->t != tnull)
		{
			printf(" . ");
			print(x->cons.cdr);
		}
		printf(")");
	}
	else
	{
		switch (x->t)
		{
			case tnull: printf("()"); break;
			case tint: printf("%i", x->i); break;
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

