#include "types.c"

int eq(atom *a, atom *b) {
	
	if (*a != *b) return 0;
	
	switch (*a) {
		
		case tint:
			return ((aint *) a)->i == ((aint *) b)->i;
		case tchar:
			return ((achar *) a)->c == ((achar *) b)->c;
		case tfun:
			return a == b;
		case tstring:
			return !strcmp(((astring *) a)->s, ((astring *) b)->s);
		case tsym:
			return !strcmp(((asym *) a)->s, ((asym *) b)->s);
		
		
		
		
	}
	
	
	
}





atom *lookup(nspace *search, asym name) {
	
	if (!search) return NULL;
	if (search->)
	
	
}