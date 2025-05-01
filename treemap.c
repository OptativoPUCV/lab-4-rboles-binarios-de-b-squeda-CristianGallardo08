#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode
{
    Pair *pair;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
};

struct TreeMap
{
    TreeNode *root;
    TreeNode *current;
    int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2)
{
    if (tree->lower_than(key1, key2) == 0 &&
        tree->lower_than(key2, key1) == 0)
        return 1;
    else
        return 0;
}

TreeNode *createTreeNode(void *key, void *value)
{
    TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL)
        return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2))
{
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));

    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;

    return new;
}

void insertTreeMap(TreeMap *tree, void *key, void *value)
{
    TreeNode *padre = NULL;
    TreeNode *nodo = tree->root;
    while (nodo != NULL)
    {
        padre = nodo;
        if (is_equal(tree, key, nodo->pair->key))
        {
            return;
        }
        if (tree->lower_than(key, nodo->pair->key))
        {
            nodo = nodo->left;
        }
        else
        {
            nodo = nodo->right;
        }
    }
    TreeNode *newNode = createTreeNode(key, value);
    newNode->parent = padre;
    if (padre == NULL)
        tree->root = newNode;
    else if (tree->lower_than(key, padre->pair->key))
        padre->left = newNode;
    else
        padre->right = newNode;

    tree->current = newNode;
}

TreeNode *minimum(TreeNode *x)
{
    if (x == NULL)
        return NULL;
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap *tree, TreeNode *node)
{
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent == NULL)
            tree->root = NULL;
        else if (node->parent->left == node)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
    }
    else if (node->left == NULL || node->right == NULL)
    {
        TreeNode *hijo = (node->left) ? node->left : node->right;
        hijo->parent = node->parent;
        if (node->parent == NULL)
            tree->root = hijo;
        else if (node->parent->left == node)
            node->parent->left = hijo;
        else
            node->parent->right = hijo;
    }
    else
    {
        TreeNode *minNode = minimum(node->right);
        node->pair = minNode->pair;
        removeNode(tree, minNode);
    }

    free(node);
}

void eraseTreeMap(TreeMap *tree, void *key)
{
    if (tree == NULL || tree->root == NULL)
        return;

    if (searchTreeMap(tree, key) == NULL)
        return;
    TreeNode *node = tree->current;
    removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key)
{
    TreeNode *nodo = tree->root;
    while (nodo != NULL)
    {
        if (is_equal(tree, key, nodo->pair->key))
        {
            tree->current = nodo;
            return nodo->pair;
        }
        if (tree->lower_than(key, nodo->pair->key))
            nodo = nodo->left;
        else
            nodo = nodo->right;
    }
    return NULL;
}

Pair *upperBound(TreeMap *tree, void *key)
{
    return NULL;
}

Pair *firstTreeMap(TreeMap *tree)
{
    if (tree->root == NULL)
        return NULL;
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair *nextTreeMap(TreeMap *tree)
{
    TreeNode *node = tree->current;

    if (node->right != NULL)
    {
        tree->current = minimum(node->right);
        return tree->current->pair;
    }

    TreeNode *parent = node->parent;
    while (parent != NULL && parent->right == node)
    {
        node = parent;
        parent = parent->parent;
    }

    tree->current = parent;
    return (parent != NULL) ? parent->pair : NULL;
}
