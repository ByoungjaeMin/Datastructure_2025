#pragma once
#ifndef TREE_H
#define TREE_H

typedef struct TreeNode {
    char name[100];             // ��� �̸� (ī�װ� �Ǵ� ��ǰ��)
    char data[100];             // ����, HS�ڵ� �� �ΰ� ����
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Ʈ�� ���� �Լ�
TreeNode* insert_node(TreeNode* root, const char* path, const char* data);
TreeNode* search_tree(TreeNode* root, const char* name);
void print_tree(TreeNode* root, int depth);
void free_tree(TreeNode* root);

#endif
