#include <stdlib.h>
#include <stdio.h>

typedef struct node_s
{
	struct node_s *next;
	int count;
} node_t;

static node_t *list;

void addNode(int count)
{
	printf("Adding node with count: %d\n", count);
	node_t *newNode;
	if (!(newNode = (node_t*)calloc(1, sizeof(node_t))))
	{
		printf("Calloc failed!\n");
		return;
	}
	newNode->count = count;
	newNode->next = list;
	list = newNode;
}


void printList()
{
        node_t *lst = list;

        while(lst)
        {
                printf("%d\n", lst->count);
                lst = lst->next;
        }
	printf("\n");
}


node_t * findNode(const int num)
{
	node_t *lst = list;
        while(lst && lst->count != num)
        {
		lst = lst->next;
        }
	return lst;
}


int deleteNode(node_t *node)
{
        printf("Deleting node with count: %d\n", node->count);
        free(node);
	return 0;
}


int removeNode(const int num)
{
	node_t *target, *lst;

	if (!(target = findNode(num)))
	{
		printf("Nothing removed.  No target for %d\n", num);
		return -1;
	}

	if (target == list)
	{
		list = target->next;
	}
	else 
	{
		for (lst = list; lst->next && lst->next != target; lst = lst->next) ;
        	lst->next = target->next;
	}
	
	return deleteNode(target);
}


int main (int argc, char ** argv)
{
	list = NULL;

	int i;
	for (i = 0; i < 10; ++i)
	{
		addNode(i);
	}

	printList();
	
	removeNode(0);
	removeNode(9);
	removeNode(4);
	removeNode(5);
	removeNode(0);
	removeNode(1);
	removeNode(3);
	removeNode(7);
	removeNode(2);
	removeNode(6);
	removeNode(8);
	removeNode(8);
	removeNode(9);
	removeNode(9);
	removeNode(0);

	addNode(10);

	removeNode(11);

	removeNode(10);


	for (i = 0; i < 100000; ++i)
	{
		int c = rand() % 10000;
		if (rand() % 2 == 0)
			addNode(c);
		else
			removeNode(c);
	}

	return 0;
}
