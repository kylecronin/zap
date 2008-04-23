#include <stdlib.h>
#include <stdio.h>

typedef enum {
	tcons, tint, tchar, tfun, tnull
} atom;


typedef struct acons {
	atom t, *car, *cdr;
} acons;

atom *newcons(atom *car, atom *cdr) {
	acons *ret = malloc(sizeof(acons));
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
	ret->i = i;
	return (atom *) ret;
}


typedef struct achar {
	atom t;
	char c;
} achar;

atom *newchar(char c) {
	achar *ret = malloc(sizeof(achar));
	ret->c = c;
	return (atom *) ret;
}

int main() {
	
	atom *a = newcons(newint(1), newint(2));
	
	printf("howdy\n");
}