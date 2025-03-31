#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "InsertLinkedList.h"

LinkedList_h* createLinkedList_h(void)
{
	LinkedList_h* L;
		L = (LinkedList_h*)malloc(sizeof(LinkedList_h));
	L->head = NULL;
	return L;
}

void freeLinkedList_h(LinkedList_h* L)
{
	ListNode* p;
	while (L->head != NULL)
	{
		p = L->head;
		L->head = L->head->link;
		free(p);
		p = NULL;
	}
}

void printList(LinkedList_h* L)
{
	ListNode* p;
	printf("L = (");
	p = L->head;
	while (p != NULL)
	{
		printf("%s", p->data);
		p = p->link;
		if (p != NULL)printf(", ");
	}
	printf(") \n");
}

void insertFirstNode(LinkedList_h* L, char* x)
{
	ListNode* newNode;
	newNode = (ListNode*)malloc(sizeof(ListNode));
	strcpy(newNode->data, x);
	newNode->link = L->head;
	L->head = newNode;
}

void insertMiddleNode(LinkedList_h* L, ListNode* pre, char* x)
{
	ListNode* newNode;
	newNode = (ListNode*)malloc(sizeof(ListNode));
	strcpy(newNode->data, x);
	if (L->head == NULL)
	{
		newNode->link = NULL;
		L->head = newNode;
	}
	else if (pre == NULL)
	{
		newNode->link = L->head;
		L->head = newNode;
	}
	else
	{
		newNode->link = pre->link;
		pre->link = newNode;
	}
}

void insertLastNode(LinkedList_h* L, char* x)
{
	ListNode* newNode;
	ListNode* temp;
	newNode = (ListNode*)malloc(sizeof(ListNode));
	strcpy(newNode->data, x);
	newNode->link = NULL;
	if (L->head == NULL)
	{
		L->head = newNode;
		return;
	}
	temp = L->head;
	while (temp->link != NULL)
		temp = temp->link;
	temp->link = newNode;
}