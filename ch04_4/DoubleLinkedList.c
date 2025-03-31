#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DoubleLinkedList.h"

LinkedList_h* createLinkedList_h(void)
{
	LinkedList_h* DL;
	DL = (LinkedList_h*)malloc(sizeof(LinkedList_h));
	DL->head = NULL;
	return DL;
}

void printList(LinkedList_h* DL)
{
	ListNode* p;
	printf("DL = (");
	p = DL->head;

	while (p != NULL)
	{
		printf("%s", p->data);
		p = p->rlink;
		if (p != NULL)
			printf(", ");
	}
	printf(") \n");
}

void insertNode(LinkedList_h* DL, ListNode* pre, char* x)
{
	ListNode* newNode;
	newNode = (ListNode*)malloc(sizeof(ListNode));
	strcpy(newNode->data, x);
	if (DL->head == NULL)
	{
		newNode->rlink = NULL;
		newNode->llink = NULL;
		DL->head = newNode;
	}
	else
	{
		newNode->rlink = pre->rlink;
		pre->rlink = newNode;
		newNode->llink = pre;
		if (newNode->rlink != NULL)
			newNode->rlink->llink = newNode;
	}
}
void deleteNode(LinkedList_h* DL, ListNode* old)
{
	if (DL->head == NULL)
		return;
	else if (old == NULL)
		return;
	else
	{
		old->llink->rlink = old->rlink;
		old->rlink->llink = old->llink;
		free(old);
	}
}

ListNode* searchNode(LinkedList_h* DL, char* x)
{
	ListNode* temp;
	temp = DL->head;
	while (temp != NULL)
	{
		if (strcmp(temp->data, x) == 0)
			return temp;
		else temp = temp->rlink;
	}
	return temp;
}