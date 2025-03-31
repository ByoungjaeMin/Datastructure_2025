#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CircularLinkedList.h"

LinkedList_h* createLinkedList_h(void)
{
	LinkedList_h* CL;
	CL = (LinkedList_h*)malloc(sizeof(LinkedList_h));
	CL->head = NULL;
	return CL;
}

void printList(LinkedList_h* CL)
{
	ListNode* p;
	printf("CL = (");
	p = CL->head;
	if (p == NULL)
	{
		printf(") \n");
		return;
	}
	do
	{
		printf("%s", p->data);
		p = p->link;
		if (p != CL->head)
			printf(", ");
	} while (p != CL->head);
	printf(") \n");
}

void insertFirstNode(LinkedList_h* CL, char* x)
{
	ListNode* newNode, * temp;
	newNode = (ListNode*)malloc(sizeof(ListNode));
	strcpy(newNode->data, x);
	if (CL->head == NULL)
	{
		CL->head = newNode;
		newNode->link = newNode;
	}
	else
	{
		temp = CL->head;
		while (temp->link != CL->head)
			temp = temp->link;
		newNode->link = temp->link;
		temp->link = newNode;
		CL->head = newNode;
	}
}

void insertMiddleNode(LinkedList_h* CL, ListNode* pre, char* x)
{
	ListNode* newNode;
	newNode = (ListNode*)malloc(sizeof(ListNode));
	strcpy(newNode->data, x);
	if (CL->head == NULL)
	{
		CL->head = newNode;
		newNode->link = newNode;
	}
	else
	{
		newNode->link = pre->link;
		pre->link = newNode;
	}
}

void deleteNode(LinkedList_h* CL, ListNode* old)
{
	ListNode* pre;
	if (CL->head == NULL)
		return;
	if (CL->head->link == CL->head)
	{
		free(CL->head);
		CL->head = NULL;
		return;
	}
	else if (old == NULL)
		return;
	else
	{
		pre = CL->head;
		while (pre->link != old)
			pre = pre->link;
		pre->link = old->link;
		if (old == CL->head)
			CL->head = old->link;
		free(old);
	}
}

ListNode* searchNode(LinkedList_h* CL, char* x)
{
	ListNode* temp;
	temp = CL->head;
	if (temp == NULL)
		return NULL;
	do
	{
		if (strcmp(temp->data, x) == 0)
			return temp;
		else 
			temp = temp->link;
	} while (temp != CL->head);
	return NULL;
}