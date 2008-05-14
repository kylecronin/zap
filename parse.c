#include <stdio.h>
#include "types.c"

#define BUFFSIZE 16384

atom *read (FILE*);
static char *next (void);
static atom *parse (void);
static atom *parselist (void);

static char *p;

/*
- read a line
- check () balance (and "" balance)
	- if too many (, read another line
	- if too many ), error out
- flatten whitespace (perhaps insert \0)
- flatten comments (from ; to \n)
- convert case (toupper/tolower)
- begin recursive parse

I would like to do everything in a single pass. In order to compensate for
various whitespace, 
*/

atom *read (FILE *input) {
	static char buffer[BUFFSIZE], *store;
	char obuff[128], *obp = obuff;
	char *rp, *wp; /* read pointer, write pointer */
	int paren, str, offset, cmnt;
	
	if (store) {
		fgets(buffer, BUFFSIZE, input);
		store = buffer;
	}
	
	paren = str = cmnt = 0;
	for (rp = wp = buffer; *rp; rp++, wp++) {
		if (str)
			if (*t == '"')
				str = 0;
			else;
		else switch (*t) {
			case '"':
				str = 1;
				break;
			case '(':
				if (offset <= 0) {
					*(++obuff) = *t;
					
					
				}
				
			
			
			
		}
		
		
		
			
		
		
	}
	
	
	
}




/* moves p to the next item, returns the old value for p */
static char *next (void) {
	char *ret = p;
	while (p++);
	return ret;
}

/* parses a list */
static atom *parselist (void) {
	atom *ret, *car, *cdr;
	
	switch (*next()) {
		case ')':
			return NULL;
		case '.':
			ret = rparse();
			if (*p == ')')
				next();
			else
				printf("invalid dotted list\n");
			return ret;
			break;
		default:
			car = rparse();
			cdr = parselist();
			return newcons(car, cdr);
	}
}

static atom *rparse (void) {
	char *t;
	
	if (!*p)
		return NULL; /* we've reached the end */
	
	switch (*p) {
		case '(':
			return next(), parselist();
		case '"':
			t = next() + 1;
			*(p-2) = '\0';
			return newstring(t);
		case '#':
			switch (*(p+1)) {
				case '\\':
					return t = next(), newchar(*(t+2));
				case 't':
					return next(), newbool(1);
				case 'f':
					return next(), newbool(0);
				default:
					printf("invalid # syntax\n");
			}
			break;
		case '\'':
			return next(), newcons(newsym("quote"), newcons(rparse(), NULL));
		default:
			if (isdigit(*p))
				return t = next(), newint(atoi(t));
			else
				return t = next(), newsym(t);
	}
}




main () {}