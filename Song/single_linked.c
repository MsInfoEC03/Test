#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


static struct node *creat_linked(int dat);
void creat_tail(struct node *p, int dat);

struct node
{
	int data;
	struct node *pnext;
};

static struct node *creat_linked(int dat)
{
	struct node *p = (struct node *)malloc(sizeof(struct node));
	if(NULL == p)
	{
		printf("creat_linked error.\n");
		return NULL;
	}
	bzero(p,sizeof(struct node));

	p->data = dat;
	p->pnext= NULL;
	return p;
}

void creat_tail(struct node *p, int dat)
{
	
	while(NULL != p->pnext)
	{
		p=p->pnext;
	}
	p->pnext = creat_linked(dat);
}

int main(void)
{
	struct node *pHeader = creat_linked(666);

	pHeader->pnext = creat_linked(777);
	//pHeader->pnext->pnext = creat_linked(888);
	creat_tail(pHeader, 1000);

	printf("1 linked = %d\n",pHeader->data);
	printf("2 linked = %d\n",pHeader->pnext->data);
	printf("3 linked = %d\n",pHeader->pnext->pnext->data);
	return 0;
}






