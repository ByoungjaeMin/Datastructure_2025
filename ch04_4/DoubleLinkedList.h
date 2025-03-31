#pragma once
typedef struct ListNode
{
	struct ListNode* llink;
	char data[4];
	struct ListNode* rlink;
}
ListNode;

typedef struct
{
	ListNode* head;
}
LinkedList_h;

LinkedList_h* createLinkedList_h(void);
void printList(LinkedList_h* DL);
void insertNode(LinkedList_h* DL, ListNode* pre, char* x);
void deleteNode(LinkedList_h* DL, ListNode* old);
ListNode* searchNode(LinkedList_h* DL, char* x);