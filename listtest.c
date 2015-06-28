#include <stdio.h>
#ifndef __cplusplus
#	include <stdbool.h>
#endif

// bare bone double-linked intrusive list implementation using sentinel element
// written by Daniel Balster (this source was started somewhen in 1989 on Amiga)

typedef struct _DList DList;

struct _DList
{
	DList *next;
	DList *prev;
};

// simplify typing.
#define dlist_foreach(_type, _list,_iter) if (!dlist_is_empty(_list)) for (_type *_iter=(_type*)(_list)->next; (DList*)_iter!=_list; _iter=(_type*)(((DList*)_iter)->next))

bool dlist_is_empty(const DList *const _list)
{
	return _list && _list==_list->next && _list==_list->prev;
}

void dlist_init(DList *const _list)
{
	_list->next = _list->prev = _list;
}

void dlist_append(DList *const _list, DList *const _elem)
{
	_elem->next = _list;
	_elem->prev = _list->prev;
	_elem->prev->next = _elem;
	_list->prev = _elem;
}

void dlist_prepend(DList *const _list, DList *const _elem)
{
	_elem->next = _list->next;
	_list->next = _elem;
	_elem->prev = _list;
	_elem->next->prev = _elem;
}

DList *dlist_remove(DList *const _elem)
{
	_elem->prev->next = _elem->next;
	_elem->next->prev = _elem->prev;
	_elem->next = _elem;
	_elem->prev = _elem;
	return _elem;
}

DList *dlist_remove_first(DList *const _list)
{
	DList *elem = _list->next;
	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;
	elem->next = elem;
	elem->prev = elem;
	return elem;
}

DList *dlist_remove_last(DList *const _list)
{
  DList *elem = _list->prev;
  elem->prev->next = elem->next;
  elem->next->prev = elem->prev;
  elem->next = elem;
  elem->prev = elem;
  return elem;
}

void dlist_reverse(DList *const _list)
{
  DList *iter = _list;
  do
  {
    DList *tmp = iter->next;
    iter->next = iter->prev;
    iter->prev = tmp;
    iter = tmp;
  }
  while (iter!=_list);
}

// unit test

typedef struct
{
	DList node;
	const char *content;
} MyNode;

int main( int argc, char **argv )
{
	DList list;
	MyNode A,B,C,D;
	A.content = "A";
	B.content = "B";
	C.content = "C";
	D.content = "D";
	
	dlist_init(&list);
	dlist_append(&list,&A.node);
	dlist_prepend(&list,&B.node);
	dlist_prepend(&B.node,&C.node);
	dlist_append(&C.node,&D.node);
	
	dlist_foreach(MyNode,&list,iter) printf("%s\n",iter->content);

	dlist_reverse(&list);

	printf("reversed\n");
	dlist_foreach(MyNode,&list,iter) printf("%s\n",iter->content);
	
	dlist_remove(&C.node);

	printf("removed C\n");
	dlist_foreach(MyNode,&list,iter) printf("%s\n",iter->content);

	return 0;
}