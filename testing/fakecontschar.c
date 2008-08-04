#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

char *pbos;

/**
 * Continuation datastructure.
**/
typedef struct cont {
	jmp_buf registers;
	int n;
	char *stack;
	/*
	 * Pointer to next continuation in chain.
	 */
	struct cont *next;
} cont;

void save_stack(cont *c,char *pbos,char *ptos) {
	int n = pbos-ptos;
	int i;

	c->stack = (char *)malloc(n*sizeof(char));
	c->n = n;
	for (i = 0; i<n; ++i)
		c->stack[i] = pbos[-i];
}

cont *getcontext() {
	cont *c = (cont *)malloc(sizeof(cont));
	char tos;
	/*
	 * Save registers
	 */
	if (!setjmp(c->registers)) {
		save_stack(c,pbos,&tos);
		return c;
	} else 
		return 0;
}

cont *gcont = 0;

/**
 * Save current continuation.
**/
int save_context() {
	cont *c = getcontext();
	if (c) {
		c->next = gcont;
		gcont = c;
		return 0;
	} else
		return 1;
}

void restore_stack(cont *c,int once_more) {
  char padding[12];
	char tos;
	int i,n;
	/*
	 * Make sure there's enough room on the stack
	 */
	if (pbos-c->n<&tos)
		restore_stack(c,1);

	if (once_more)
		restore_stack(c,0);

	/*
	 * Copy stack back out from continuation
	 */
	n = c->n;
	for (i = 0; i<n; ++i) {
		pbos[-i] = c->stack[i];
	}
	longjmp(c->registers,1);
}

/**
 * Restore last continuation.
**/
void restore_context() {
	if (gcont) {
		cont *old = gcont;
		gcont = old->next;
		restore_stack(old,1);
	} else 
		exit(1);
}

int integer(int m,int n) {
	int i;

	for (i = m; i<=n; ++i)
		if (!save_context())
			return i;

	restore_context();
}

int main() {
	char bos;
	pbos = &bos;
	
	int i,j;

	i = integer(2,100);
	j = integer(2,i);

	if (i*j!=6789) 
		restore_context();

	printf("%d %d\n",i,j);
}