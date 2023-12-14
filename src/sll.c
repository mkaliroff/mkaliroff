#include <stdlib.h> /* malloc, realloc, free */
#include <assert.h> /* assert.h */

#include "sll.h" /* api */


/*****************************************************************************/

typedef struct sll_node
{
	void *data;
	struct sll_node *next;
}  sll_node_t;

struct list
{
	sll_node_t *head;
	sll_node_t *tail;
};

int SLLCounter(void *data, void *parameter);

/*****************************************************************************/

sll_t *SLLCreate(void)
{
	sll_t *list = (sll_t*)malloc(sizeof(sll_t));
	if(NULL == list)
	{
		return (NULL);
	}
	
	list -> head = (sll_node_t*)malloc(sizeof(sll_node_t));
	if(NULL == list -> head)
	{
		free(list);
		return (NULL);
	}
	
	list -> tail = list -> head;
	list -> head -> data = list;
	list -> head -> next = NULL;

	return (list);
}

/*****************************************************************************/

void SLLDestroy(sll_t *sll)
{
	sll_iter_t iter = sll -> head;
	sll_iter_t to_free = iter;
	
	assert(sll);
	
	while(iter != NULL)
	{
		to_free = iter;
		iter = iter -> next;
		free(to_free);
	}
	
	free(sll);
	sll = NULL;
}

/*****************************************************************************/

size_t SLLCount(const sll_t *sll)
{
	size_t count = 0;
	
	assert(sll);
	
	SLLForEach(SLLBegin(sll), SLLEnd(sll), SLLCounter, &count);
	return (count);
}

int SLLCounter(void *data, void *parameter)
{
	size_t *count = (size_t*)parameter;
	
	assert(count);
	
	(*count)++;
	data = (void*)data;
	
    return (0);
}

/*****************************************************************************/

sll_iter_t SLLEnd(const sll_t *sll)
{
	assert(sll);
	
	return (sll -> tail);
}

/*****************************************************************************/

sll_iter_t SLLBegin(const sll_t *sll)
{
	assert(sll);
	
	return (sll -> head);
}

/*****************************************************************************/

int SLLIsEmpty(const sll_t *sll)
{
	assert(sll);
	
	return (SLLIterIsEqual(SLLBegin(sll), SLLEnd(sll)));
}

/*****************************************************************************/

int SLLIterIsEqual(sll_iter_t iter1, sll_iter_t iter2)
{
	assert(iter1);
	assert(iter2);
	
	return (iter1 == iter2);
}

/*****************************************************************************/

void SLLSetData(sll_iter_t iterator, void *data)
{
	assert(iterator);
	
	iterator -> data = data;
}

/*****************************************************************************/

void *SLLGetData(sll_iter_t iterator)
{
	assert(iterator);
	
	return (iterator -> data);
}

/*****************************************************************************/

void SLLRemove(sll_iter_t iterator)
{
	sll_iter_t to_free = NULL;
	
	assert(iterator);
	
	to_free = iterator -> next;
	
	iterator -> data = SLLNext(iterator) -> data;
	iterator -> next = SLLNext(iterator) -> next;
	free(to_free);
	
	if(NULL == SLLNext(iterator))
	{
		((sll_t*)(iterator -> data)) -> tail = iterator;
	}
}

/*****************************************************************************/

sll_iter_t SLLNext(sll_iter_t iterator)
{
	assert(iterator);
	
	return (iterator -> next);
}

/*****************************************************************************/

sll_iter_t SLLInsert(sll_iter_t iterator, void *data)
{
	sll_iter_t new_node = NULL;
	
	assert(iterator);
	
	new_node = malloc(sizeof(sll_node_t));
	if(NULL == new_node)
	{
		while(iterator -> next)
		{
			iterator = iterator -> next; 
		}
		return (iterator);
	}
		
	new_node -> data = iterator -> data;
	new_node -> next = iterator -> next;
	
	iterator -> data = data;
	iterator -> next = new_node;

	if(NULL == new_node -> next)
	{
		((sll_t*)(new_node -> data)) -> tail = new_node;
	}
	
	return (new_node);
}

/*****************************************************************************/

void SLLAppend(sll_t *dest, sll_t *source)
{	
	assert(dest);
	assert(source);
	
	dest -> tail -> data = source -> head -> data;
	dest -> tail -> next = source -> head -> next;
	
	dest -> tail = source -> tail;
	dest -> tail -> data = dest;
	
	source -> head -> next = NULL;
	source -> head -> data = source;
	source -> tail = source -> head;

}

/*****************************************************************************/

sll_iter_t SLLFind(sll_iter_t from, sll_iter_t to, sll_compare_func_t cmp, void *parameter)
{
	assert(from);
	assert(to);
	
	while (from != to)
	{
		if (0 == cmp(from -> data, parameter))
		{
			return (from);
		}
		from = from -> next;
	}
	
	return (to);
}

/*****************************************************************************/

int SLLForEach(sll_iter_t from, sll_iter_t to, sll_action_func_t action, void *parameter)
{
	int result = 0;
	
	assert(from);
	assert(to);
	
	while (from != to)
	{
		result = action(from -> data, parameter);
		if (0 !=  result)
		{
			return (result);
		}
		from = from -> next;
	}
	
	return (0);
}

/*****************************************************************************/
