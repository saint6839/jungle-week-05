#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

void preorder_traversal(node_t *node)
{
    if (node != NULL)
    {
        printf("%d ", node->key);
        preorder_traversal(node->left);
        preorder_traversal(node->right);
    }
}

int main(int argc, char *argv[])
{
    rbtree *t = new_rbtree();
    rbtree_insert(t, 10);
    rbtree_insert(t, 20);
    rbtree_insert(t, 10);
    rbtree_insert(t, 30);
    
    // delete_rbtree(t);
    node_t *z = rbtree_find(t, 30);
    
    return 0;
}