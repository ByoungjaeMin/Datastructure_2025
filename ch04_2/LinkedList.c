#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

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
		if (p != NULL)
			printf(", ");
	}
	printf(")\n");
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
	if (L == NULL)
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

void deleteNode(LinkedList_h* L, ListNode* p)
{
	ListNode* pre;
	if (L->head == NULL)
		return;
	if (L->head->link == NULL)
	{
		free(L->head);
		L->head = NULL;
		return;
	}
	else if (p == NULL)
		return;
	else
	{
		pre = L->head;
		while (pre->link != p)
			pre = pre->link;
		pre->link = p->link;
		free(p);
	}
}

ListNode* searchNode(LinkedList_h* L, char* x)
{
	ListNode* temp;
	temp = L->head;
	while (temp != NULL)
	{
		if (strcmp(temp->data, x) == 0)
			return temp;
		temp = temp->link;
	}
	return temp;
}

void reverse(LinkedList_h* L)
{
	ListNode* p;
	ListNode* q;
	ListNode* r;
	p = L->head;
	q = NULL;

	while (p != NULL)
	{
		r = q;
		q = p;
		p = p->link;
		q->link = r;
	}
	L->head = q;
}