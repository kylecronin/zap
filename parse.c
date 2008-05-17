#include <stdio.h>
#include "types.c"

#define BUFFSIZE 256

atom *read (FILE*);
static char readchar (void);
static void putback (char c);
static char *next (void);
static atom *parse (char*);
static atom *parselist (void);

FILE *i;

atom *read (FILE *input) {
	i = input;
	return parse(next());
}

static char readchar (void) {
	int c = getc(i);
	if (c != EOF)
		return c;
	printf("read error: unexpected EOF (check parens)\n");
	exit(1);
}

static void putback (char c) {
	ungetc(c, i);
}

static char *next (void) {
	static char buff[BUFFSIZE];
	char *c = buff;
	while (isspace(*c = readchar()));
	
	if (*c != ')' && *c != '(' && *c != '"') {
		while( !isspace(*(c+1) = readchar()) && *(c+1) != '(' && *(c+1) != ')')
			c++;
		putback(*(c+1));
	}
	
	else if (*c == '"') {
		while ( (*(++c) = readchar()) != '"' )
			if (*c == '\\')
				*(++c) = readchar();
	}
	
	*(c+1) = '\0';

	return buff;
}


static atom *parselist (void) {
	char *p = next();	
	atom *ret, *car, *cdr;
	
	switch (*p) {
		case ')':
			return NULL;
		case '.':
			ret = parse(next());
			if (*next() == ')')
				return ret;
			else
				printf("invalid dotted list\n");
				exit(1);
			break;
		default:
			car = parse(p);
			cdr = parselist();
			return newcons(car, cdr);
	}
}

static atom *parse (char *p) {
	char *t = p;
	
	if (!*p)
		return NULL; /* we've reached the end */
	
	switch (*p) {
		case '(':
			return parselist();
		case '"':
			while (*(++t));
			*(t-1) = '\0';
			return newstring(p+1);
		case '#':
			switch (*(p+1)) {
				case '\\':
					return newchar(*(p+2));
				case 't':
					return newbool(1);
				case 'f':
					return newbool(0);
				default:
					printf("invalid # syntax\n");
					exit(1);
			}
			break;
		case '\'':
			return newcons(newsym("quote"), newcons(parse(p+1), NULL));
		case ')':
			
		default:
			if (isdigit(*p))
				return newint(atoi(p));
			else
				return newsym(p);
	}
}