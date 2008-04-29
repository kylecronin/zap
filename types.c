#include <stdlib.h>
#include <stdio.h>

/**
 * atom
 *
 * This contains all the basic atomic types in Scheme. Perhaps one day I can
 * simply create an atom of a variable amount of bytes and base all atoms on
 * that. You will notice that a null type is included - this is done so that
 * you can dereference ANY atom without checking for null. 
**/
typedef enum {
	tcons, tint, tchar, tfun, tnull
} atom;

/**
 * null
 *
 * Since a null is a null, instead of mallocing new nulls every time we will
 * just point to the same one.
**/
atom pnull = tnull;
atom *anull = &pnull;


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
	
	acons *b = (acons *) a;
	
	printf("howdy\n");
}