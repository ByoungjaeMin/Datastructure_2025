#pragma once
typedef struct ListNode
{
	char data[4];
	struct ListNode* link;
}
ListNode;

typedef struct
{
	ListNode* head;
}
LinkedList_h;

LinkedList_h* createLinkedList_h(void);
void freeLinkedList_h(LinkedList_h* L);
void printList(LinkedList_h* L);
void insertFirstNode(LinkedList_h* L, char* x);
void insertMiddleNode(LinkedList_h* L, ListNode* pre, char* x);
void insertLastNode(LinkedList_h* L, char* x);
void deleteNode(LinkedList_h* L, ListNode* p);
ListNode* searchNode(LinkedList_h* L, char* x);
void reverse(LinkedList_h* L);
