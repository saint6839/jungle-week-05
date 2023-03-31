#include "rbtree.h"

#include <stdlib.h>

rbtree* new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
  new_node->color = RBTREE_BLACK;
  new_node->key = 0;
  new_node->parent = p->nil;
  new_node->left = p->nil;
  new_node->right = p->nil;

  p->nil = new_node;
  p->root = p->nil;
  return p;
}

// memory leak 방지용 free 메서드 
void free_nodes(node_t* root, node_t* nil) {
  if (nil == root) return;
  free_nodes(root->left, nil);
  free_nodes(root->right, nil);
  free(root);
}

// rbtree 제거
void delete_rbtree(rbtree *t) {
  free_nodes(t->root, t->nil);
  free(t->nil);
  free(t);
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


// node_t *rbtree_insert(rbtree *t, const key_t key) {
//     node_t *z = (node_t*)calloc(1, sizeof(node_t));
//     z -> key = key;

//     node_t *y = t->nil;
//     node_t *x = t->root;
    
//     // 삽입할 위치를 탐색한다.
//     while (x != t->nil)
//     {
//         y = x;
//         if (z->key < x->key) {
//             x = x->left;
//         }
//         else {
//             x = x->right;
//         }
//     }
//     z->parent = y;
//     if (y == t->nil) {
//         t->root = z;
//     }
//     else if (z->key < y->key) {
//         y->left = z;
//     } else {
//         y->right = z;
//     }
//     z->left = t->nil;
//     z->right = t->nil;
//     z->color = RBTREE_RED;
    
//     // TODO: implement insert
//     return t->root;
// }

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
