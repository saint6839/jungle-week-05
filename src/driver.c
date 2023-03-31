#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
    new_node->color = RBTREE_BLACK;
    new_node->key = 0;
    new_node->parent = p->nil;
    new_node->left = p->nil;
    new_node->right = p->nil;

    p->nil = new_node;
    p->root = p->nil;
    return p;
}


// x가 부모이고 y가 오른쪽 노드일때 y를 부모로 바꾸고 x를 왼쪽 노드로 회전 시키는 함수
void left_rotate(rbtree *t, node_t *x) {
    node_t *y = x->right; // x의 오른쪽 노드의 주소를 선언함
    x->right = y->left; // y의 왼쪽 노드를 x의 오른쪽으로 옮김
    if (y->left != t->nil) { // 만약 y의 왼쪽 노드가 NIL이 아니라면,
        y->left->parent = x; // 기존 y의 왼쪽에 있던 노드의 parent 노드를 x로 맞추어준다.
    }
    y->parent = x->parent; // x의 자리에 y가 올라왔으므로, 기존 x의 부모를 y의 부모로 바꾸어준다.
    if (x->parent == t->nil) { // 만약 x의 부모가 NIL 이라면, root 노드라는 의미이므로,
        t->root = y; // y를 트리의 root로 선언한다.
    } else if (x == x->parent->left) { // 만약 x가 회전 되기 전에 부모의 왼쪽에 위치한 노드였다면,
        x->parent->left = y; // x부모의 왼쪽을 y로 바꾸어준다.
    } else { // 아니라면, x가 회전 되기 전에 부모의 오른쪽에 위치했다는 의미이므로, 
        x->parent->right = y; // x부모의 오른쪽을 y로 바꾸어준다.
    }
    y->left = x; // y의 왼쪽을 x로 바꾸어준다.
    x->parent = y; // x의 부모를 y로 바꾸어준다.
}

// y가 부모이고 x가 왼쪽 노드일때 x를 부모로 바꾸고 y를 오른쪽 노드로 회전 시키는 함수
void right_rotate(rbtree *t, node_t *y) {
    node_t *x = y->left; // y의 왼쪽 노드를 선언함
    y->left = x->right; // x의 오른쪽 노드를 y의 왼쪽으로 옮겨줌
    if (x->right != t->nil) { // x의 오른쪽이 NIL이 아니라면,
        x->right->parent = y; // 기존 x의 오른쪽에 있었던 노드의 parent 노드를 y로 맞추어준다.
    }
    x->parent = y->parent; // y의 자리에 x가 올라왔으므로, 기존 y의 부모를 x의 부모로 바꾸어준다.
    if (y->parent == t->nil) {  // 만약 y의 부모가 NIL 이라면, root 노드라는 의미이므로,
        t->root = x; // x를 트리의 root로 선언한다.
    } else if(y == y->parent->left) { // 만약 y가 회전 되기 전에 부모의 왼쪽에 위치한 노드였다면,
        y->parent->left = x; // y부모의 왼쪽을 x로 바꾸어준다.    
    } else { // 아니라면, y가 회전 되기 전에 부모의 오른쪽에 위치했다는 의미이므로,
        y->parent->right = x; // y부모의 오른쪽을 x로 바꾸어준다.
    }
    x->right = y; // x의 오른쪽을 y로 바꾸어준다.
    y->parent = x; // y의 부모를 x로 바꾸어준다.
}


node_t *rbtree_insert_recursive(rbtree *t, node_t *x, const key_t key) {
    if (x == t->nil) {
        node_t *z = (node_t*)calloc(1, sizeof(node_t));
        z->key = key;
        z->parent = t->nil;
        z->left = t->nil;
        z->right = t->nil;
        z->color = RBTREE_RED;
        return z;
    }

    if (key < x->key) {
        x->left = rbtree_insert_recursive(t, x->left, key);
        x->left->parent = x;
    }
    else {
        x->right = rbtree_insert_recursive(t, x->right, key);
        x->right->parent = x;
    }
    return x;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *root = rbtree_insert_recursive(t, t->root, key);
    root->color = RBTREE_BLACK;
    t->root = root;
    return t->root;
}

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
    rbtree *new_tree = new_rbtree();
    rbtree_insert(new_tree, 10);
    rbtree_insert(new_tree, 20);
    rbtree_insert(new_tree, 30);
    node_t *n = rbtree_insert(new_tree, 40);

    preorder_traversal(n);
    return 0;
}