#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

// '/'�� ���е� ��θ� ���� ����
TreeNode* insert_node(TreeNode* root, const char* path, const char* data) {
    if (!path || *path == '\0') return root;

    char path_copy[256];
    strcpy(path_copy, path);
    char* token = strtok(path_copy, "/");
    TreeNode** current = &root;

    while (token != NULL) {
        TreeNode* p = *current;
        TreeNode* prev = NULL;

        // ���� ���ؿ��� �˻�
        while (p && strcmp(p->name, token) != 0) {
            prev = p;
            p = p->right;
        }

        if (!p) {
            // ��尡 ������ ���� ����
            p = (TreeNode*)malloc(sizeof(TreeNode));
            strcpy(p->name, token);
            p->data[0] = '\0';
            p->left = NULL;
            p->right = NULL;

            if (prev)
                prev->right = p;
            else
                *current = p;
        }

        current = &p->left;
        token = strtok(NULL, "/");
    }

    // ���� ��忡 data ����
    strcpy((*current)->data, data);
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