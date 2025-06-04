#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

// '/'로 구분된 경로를 따라 트리에 삽입하는 함수
TreeNode* insert_node(TreeNode* root, const char* path, const char* data) {
    if (!path || *path == '\0') return root;

    char path_copy[256];
    strcpy(path_copy, path);
    char* token = strtok(path_copy, "/");

    TreeNode** current = &root;
    TreeNode* lastNode = NULL;

    while (token != NULL) {
        TreeNode* p = *current;
        TreeNode* prev = NULL;

        // 같은 레벨(형제 노드)에서 token과 이름이 같은 노드가 있는지 검색
        while (p && strcmp(p->name, token) != 0) {
            prev = p;
            p = p->right;
        }

        // 없다면 새 노드 생성
        if (!p) {
            p = (TreeNode*)malloc(sizeof(TreeNode));
            if (!p) {
                perror("메모리 할당 실패");
                exit(1);
            }
            strcpy(p->name, token);
            p->data[0] = '\0';
            p->left = NULL;
            p->right = NULL;

            if (prev)
                prev->right = p;
            else
                *current = p;
        }

        // 마지막으로 생성 또는 탐색된 노드를 저장
        lastNode = p;

        // 다음 token은 이 노드의 left 서브트리 쪽으로 내려감
        current = &p->left;
        token = strtok(NULL, "/");
    }

    // 반복문 종료 후, lastNode가 NULL이 아니면 data를 복사
    if (lastNode) {
        strcpy(lastNode->data, data);
    }
    return root;
}

TreeNode* search_tree(TreeNode* root, const char* name) {
    if (!root) return NULL;
    if (strcmp(root->name, name) == 0) return root;

    TreeNode* found = search_tree(root->left, name);
    if (found) return found;

    return search_tree(root->right, name);
}

void print_tree(TreeNode* root, int depth) {
    while (root) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("- %s (%s)\n", root->name, root->data);
        print_tree(root->left, depth + 1);
        root = root->right;
    }
}

void free_tree(TreeNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
