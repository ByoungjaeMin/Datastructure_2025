#pragma once
#ifndef TREE_H
#define TREE_H

typedef struct TreeNode {
    char name[100];             // 노드 이름 (카테고리 또는 상품명)
    char data[100];             // 세율, HS코드 등 부가 정보
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 트리 조작 함수
TreeNode* insert_node(TreeNode* root, const char* path, const char* data);
TreeNode* search_tree(TreeNode* root, const char* name);
void print_tree(TreeNode* root, int depth);
void free_tree(TreeNode* root);

#endif
