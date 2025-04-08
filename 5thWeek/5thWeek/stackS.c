#include <stdio.h>
#include "stackS.h"

int top = -1;

int isStackEmpty() {
	if (top == -1) return 1;
	else return 0;
}
int isStackFull() {
	if (top == STACK_SIZE - 1) return 1;
	else return 0;
}

void push(element item) {
	if (isStackFull()) {
		printf("Stack is FULL!!\n");
		return;
	}
	else stack[++top] = item;
}

element pop() {
	if (isStackEmpty()) {
		printf("Stack is Empty!!\n");
		return 0;
	}
	else return stack[top--];
}

element peek() {
	if (isStackEmpty()) {
		printf("Stack is Empty!!\n");
		exit(1);
	}
	else return stack[top];
}

void printStack() {
	int i;
	printf("\n STACK [ ");
	for (i = 0; i <= top; i++)
		printf("%d ", stack[i]);
	printf("]");
}