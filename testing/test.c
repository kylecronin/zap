#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>

char *bos;
int perm;

typedef struct
{
	char *stack, *respt;
	int size;
	jmp_buf registers;
} cont;

cont *c;

int save()
{
	char tos;
	c = malloc(sizeof(cont));
	c->size = bos - &tos;
	c->stack = malloc(c->size);
	c->respt = &tos;
	int i = c->size;
	char *from = &tos, *to = c->stack;
	while (i--)
		*(from++) = *(to++);
	//memcpy(c->stack, &tos, c->size);
	if (setjmp(c->registers))
		return perm;
	else
		return 1;
}

void restore(int ret)
{
	char tos;
	if (c->respt < &tos)
	{
		printf("too high");
		restore(ret);
		return;
	}
	
	perm = ret;
	int i = c->size;
	char *from = c->stack, *to = c->respt;
	while (i--)
		*(from++) = *(to++);
	//memcpy(c->respt, c->stack, c->size);
	longjmp(c->registers, 0);
}

void rdepth(int d)
{
	char nothing;
	if (d) rdepth(d-1);
		else restore(5);
}

int sdepth(int d)
{
	char nothing;
	int ret;
	if (d)
	{
		printf("sd %i down\n", d);
		ret = sdepth(d-1);
		printf("sd %i up\n", d);
		return ret;
	}
	else
	{
		printf("sd bottom down\n");
		ret = save();
		printf("sd bottom up\n");
		return ret;
	}
	
}

int ims()
{
	printf("intermediate save\n");
	return sdepth(11);
}

void imr(int ret)
{
	printf("intermediate restore\n");
	rdepth(10);
}

int main (void)
{
	char bosv;
	bos = &bosv;
	
	int num = ims();
	printf("num: %i\n", num);
	
	if (num == 1)
		imr(2);
		
	printf("done\n");
	
	return 0;
}