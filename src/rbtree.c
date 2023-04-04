#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
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

// memory leak 방지용 free 메서드
void free_nodes(node_t *root, node_t *nil)
{
  if (nil == root)
    return;
  free_nodes(root->left, nil);
  free_nodes(root->right, nil);
  free(root);
}

// rbtree 제거
void delete_rbtree(rbtree *t)
{ 
  if (t == NULL) {
    return;
  }
  free_nodes(t->root, t->nil);
  free(t->nil);
  free(t);
}

// x가 부모이고 y가 오른쪽 노드일때 y를 부모로 바꾸고 x를 왼쪽 노드로 회전 시키는 함수
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right; // x의 오른쪽 노드의 주소를 선언함
  x->right = y->left;   // y의 왼쪽 노드를 x의 오른쪽으로 옮김
  if (y->left != t->nil)
  {                      // 만약 y의 왼쪽 노드가 NIL이 아니라면,
    y->left->parent = x; // 기존 y의 왼쪽에 있던 노드의 parent 노드를 x로 맞추어준다.
  }
  y->parent = x->parent; // x의 자리에 y가 올라왔으므로, 기존 x의 부모를 y의 부모로 바꾸어준다.
  if (x->parent == t->nil)
  {              // 만약 x의 부모가 NIL 이라면, root 노드라는 의미이므로,
    t->root = y; // y를 트리의 root로 선언한다.
  }
  else if (x == x->parent->left)
  {                      // 만약 x가 회전 되기 전에 부모의 왼쪽에 위치한 노드였다면,
    x->parent->left = y; // x부모의 왼쪽을 y로 바꾸어준다.
  }
  else
  {                       // 아니라면, x가 회전 되기 전에 부모의 오른쪽에 위치했다는 의미이므로,
    x->parent->right = y; // x부모의 오른쪽을 y로 바꾸어준다.
  }
  y->left = x;   // y의 왼쪽을 x로 바꾸어준다.
  x->parent = y; // x의 부모를 y로 바꾸어준다.
}

// y가 부모이고 x가 왼쪽 노드일때 x를 부모로 바꾸고 y를 오른쪽 노드로 회전 시키는 함수
void right_rotate(rbtree *t, node_t *y)
{
  node_t *x = y->left; // y의 왼쪽 노드를 선언함
  y->left = x->right;  // x의 오른쪽 노드를 y의 왼쪽으로 옮겨줌
  if (x->right != t->nil)
  {                       // x의 오른쪽이 NIL이 아니라면,
    x->right->parent = y; // 기존 x의 오른쪽에 있었던 노드의 parent 노드를 y로 맞추어준다.
  }
  x->parent = y->parent; // y의 자리에 x가 올라왔으므로, 기존 y의 부모를 x의 부모로 바꾸어준다.
  if (y->parent == t->nil)
  {              // 만약 y의 부모가 NIL 이라면, root 노드라는 의미이므로,
    t->root = x; // x를 트리의 root로 선언한다.
  }
  else if (y == y->parent->left)
  {                      // 만약 y가 회전 되기 전에 부모의 왼쪽에 위치한 노드였다면,
    y->parent->left = x; // y부모의 왼쪽을 x로 바꾸어준다.
  }
  else
  {                       // 아니라면, y가 회전 되기 전에 부모의 오른쪽에 위치했다는 의미이므로,
    y->parent->right = x; // y부모의 오른쪽을 x로 바꾸어준다.
  }
  x->right = y;  // x의 오른쪽을 y로 바꾸어준다.
  y->parent = x; // y의 부모를 x로 바꾸어준다.
}

// node_t *rbtree_insert_recursive(rbtree *t, node_t *x, const key_t key)
// {
//   if (x == t->nil)
//   {
//     node_t *z = (node_t *)calloc(1, sizeof(node_t));
//     z->key = key;
//     z->parent = t->nil;
//     z->left = t->nil;
//     z->right = t->nil;
//     z->color = RBTREE_RED;
//     return z;
//   }

//   if (key < x->key)
//   {
//     x->left = rbtree_insert_recursive(t, x->left, key);
//     x->left->parent = x;
//   }
//   else
//   {
//     x->right = rbtree_insert_recursive(t, x->right, key);
//     x->right->parent = x;
//   }
//   return x;
// }

void rbtree_insert_fixup(rbtree *t, node_t *z)
{
  printf("%d", z->parent->color);
  while (z->parent->color == RBTREE_RED)
  { // 새로 삽입된 노드는 RED인데, 부모 또한 RED인 경우에 FIXUP 수행
    if (z->parent == z->parent->parent->left)
    {                                       // z의 부모가 z조부모의 왼쪽 서브트리일 경우
      node_t *y = z->parent->parent->right; // z의 삼촌(부모 형제) 노드
      if (y->color == RBTREE_RED)
      {                                        // z의 삼촌 노드가 RED일 경우
        z->parent->color = RBTREE_BLACK;       // z의 부모 색 BLACK로 변경
        z->parent->parent->color = RBTREE_RED; // z의 조부모 색 RED로 변경
        y->color = RBTREE_BLACK;               // z의 삼촌 색 BLACK으로 변경
        z = z->parent->parent;                 // z의 위치를 z의 조부모로 변경 (z의 조부모가 새롭게 RED가 된 상태이므로, z의 조부모의 부모와 RED-RED 상태가 됐을 가능성 고려)
      }
      else
      { // z의 삼촌 노드가 BLACK일 경우
        if (z == z->parent->right)
        { // z가 z부모의 오른쪽 서브 트리일 경우
          z = z->parent;
          left_rotate(t, z); // z의 부모를 기준으로 왼쪽 회전 시켜서, 트리의 균형을 맞춘다.
        }
        z->parent->color = RBTREE_BLACK;       // z의 부모색을 BLACK으로 변경
        z->parent->parent->color = RBTREE_RED; // z의 조부모의 색을 RED로 변경
        right_rotate(t, z->parent->parent);    // z의 조부모를 기준으로 오른쪽 회전 시켜서, 트리의 균형을 맞춘다.
      }
    }
    else
    { // z의 부모가 z조부모의 오른쪽 서브트리일 경우
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      { // z의 삼촌 노드가 RED일  경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        y->color = RBTREE_BLACK;
        z = z->parent->parent;
      }
      else
      { // z의 삼촌 노드가 BLACK일 경우
        if (z == z->parent->left)
        { // z의 위치가 z부모의 왼쪽일 경우
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{

  node_t *y = t->nil;
  node_t *x = t->root;

  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  while (x != t->nil)
  {
    y = x; // y = x가 이동하며 탐색한 z가 삽입될 위치
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }

  
  z->parent = y;

  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  // red-red인 경우 찾아서 노드 위치 수정
  rbtree_insert_fixup(t, z);
  return t->root;
}

// 특정 key와 일치하는 노드 주소를 찾아 반환하는 함수
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  if (t->root == t->nil) {
    return NULL;
  }
  node_t *x = t->root;

  while (x != t->nil) {
    if (x->key > key) {
      x = x->left;
    } else if (x->key < key){
      x = x->right;
    } else {
      return x;
    }
  }

  return NULL;
}

// rbtree의 최솟값 찾는 함수
node_t *rbtree_min(const rbtree *t)
{
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (x->left == t->nil) {
      return x;
    }
    x = x->left;
  }
  
  return t->root;
}

// rbtree의 최댓값 찾는 함수
node_t *rbtree_max(const rbtree *t)
{
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (x->right == t->nil) {
      return x;
    }
    x = x->right;
  }
  
  return t->root;
}

// 삭제 대상 노드의 오른쪽 서브트리의 successor 노드를 찾아 반환
node_t *tree_minimum(rbtree *t, node_t *y)
{
  while (y->left != t->nil)
  {
    y = y->left;
  }

  return y;
}

// u노드의 위치를 v노드로 교체해주는 함수
void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void *rbtree_erase_fixup(rbtree *t, node_t *x)
{
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK) // x가 root가 아니고, x의 색이 BLACK일 동안
  {
    if (x == x->parent->left) // doubly black인 x가 왼쪽 자식일 경우
    {
      w = x->parent->right; // w는 x의 형제 노드
      if (w->color == RBTREE_RED)
      {                                // x의 오른쪽 형제 w가 RED인 경우 -> case 1) doubly black의 형제가 RED인 경우에 해당
        w->color = RBTREE_BLACK;       // 형제의 색을 BLACK으로 변경하고
        x->parent->color = RBTREE_RED; // 형제의 부모의 색을 RED로 변경한다.
        left_rotate(t, x->parent);     // 부모를 기준으로 왼쪽 회전 수행
        w = x->parent->right;          // x의 위치가 바뀌었으므로, x의 형제인 w의 위치 또한 갱신해줌
      } // 이후 경우1은 경우 2,3,4 중 하나로 변환 되었음
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) // 형제의 자식이 모두 BLACK일 경우 case 2) doubly black의 형제의 자식이 모두 BLACK일 경우에 해당
      {
        w->color = RBTREE_RED; // 형제의 검은색을 부모에게 주므로 기존 BLACK에서, RED로 변함
        x = x->parent;         // x의 부모가 x의 extra black을 물려받아, 새로운 red and black 또는 doubly black이 됨
      }
      else // 형제의 자식 중 RED인 자식이 있을 경우 case 3)에 해당, 회전을 통해 case 4)로 변경해주어야함
      {
        if (w->right->color == RBTREE_BLACK) // 형제 자식의 오른쪽 노드가 BLACK이고, 왼쪽 노드가 RED일 경우
        {
          w->left->color = RBTREE_BLACK; // 형제의 왼쪽 노드를 RED -> BLACK으로 변경
          w->color = RBTREE_RED;         // 형제의 색깔을 BLACK -> RED로 변경
          right_rotate(t, w);            // 오른쪽 회전 시켜서 꺾이지 않도록 함
          w = x->parent->right;          // x의 위치가 바뀌었으므로, x의 형제인 w의 위치 또한 갱신해줌
        }
        // case 4)에 해당
        w->color = x->parent->color;     // 형제의 색을 부모의 색으로 변경
        x->parent->color = RBTREE_BLACK; // 부모의 색을 BLACK으로 변경
        w->right->color = RBTREE_BLACK;  // 형제의 오른쪽 자식의 색을 BLACK으로 변경
        left_rotate(t, x->parent);               // 왼쪽 회전 시킴
        x = t->root;
      }
    }
    else // doubly black인 x가 오른쪽 자식일 경우
    {
      w = x->parent->left; // w는 x의 형제 노드
      if (w->color == RBTREE_RED)
      {                                // 형제의 색깔이 RED인 경우 -> case 1)에 해당
        w->color = RBTREE_BLACK;       // 형제의 색을 BLACK으로 변경한다.
        w->parent->color = RBTREE_RED; // 형제의 부모의 색을 RED로 변경한다.
        right_rotate(t, x->parent);    // 부모를 기준으로 오른쪽 회전 수행
        w = x->parent->left;           // x의 위치가 바뀌었으므로, x의 형제인 w의 위치 또한 갱신해준다.
      } // 이후 경우1은 경우 2,3,4 중 하나로 변환 되었음
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) // case 2)에 해당
      {
        w->color = RBTREE_RED; // 형제의 검은색을 부모에게 주므로 기존 BLACK에서, RED로 변함
        x = x->parent;         // x의 부모가 x의 extra black을 물려받아, 새로운 red and black 또는 doubly black이 됨
      }
      else // 형제의 자식 중 RED인 자식이 있을 경우 case 3)에 해당, 회전을 통해 case 4)로 변경해주어야함
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK; // 형제 오른쪽 노드를 RED -> BLACK
          w->color = RBTREE_RED;          // 형제 색깔을 BLACK -> RED
          left_rotate(t, w);              // 왼쪽으로 회전
          w = x->parent->left;            // x의 위치가 바뀌었으므로, x의 형제인 w의 위치 또한 갱신해줌
        }
        // case 4)에 해당
        w->color = x->parent->color;     // 형제의 색을 부모의 색으로 변경
        x->parent->color = RBTREE_BLACK; // 부모의 색을 BLACK으로 변경
        w->left->color = RBTREE_BLACK;   // 형제의 왼쪽 자식의 색을 BLACK으로 변경
        right_rotate(t, x->parent);              // 오른쪽 회전 시킴
        x = t->root;                     
      }
    }
  }
  x->color = RBTREE_BLACK; 
}

int rbtree_erase(rbtree *t, node_t *z)
{
  node_t *y = z; // z는 삭제 대상의 주소 값, y는 자식 갯수에 따라서 그대로 z가 될 수도 있고, z의 successor의 주소가 될 수도 있다.
  node_t *x;     // y의 원래 위치로 이동시키는 용도의 노드 x. y의 오른쪽 자식 또는 자식이 없을 경우 t->nil 로 설정된다.

  color_t y_original_color = y->color; // y의 색깔은 변경될 가능성이 존재하므로, 미리 새로운 변수에 y의 원래 색깔을 저장해둔다.
  if (z->left == t->nil)
  {                                    // z의 왼쪽 노드가 없다면
    x = z->right;                      // z의 오른쪽 노드를 x로 선언한다.
    rbtree_transplant(t, z, z->right); // z의 오른쪽 노드를 z의 자리에 교체한다.
  }
  else if (z->right == t->nil)
  { // z의 오른쪽 노드가 없다면
    x = z->left;
    rbtree_transplant(t, z, z->left); // z의 왼쪽 노드를 z의 자리에 교체한다.
  }
  else // z의 자식 노드가 둘 다 있을 경우
  {
    y = tree_minimum(t, z->right); // z의 오른쪽 서브트리의 successor를 찾는다.
    y_original_color = y->color;
    x = y->right;

    if (y->parent == z)
    {                // 만약 z의 오른쪽 서브트리의 successor가 z의 오른쪽 노드라면
      x->parent = y; // x의 부모를 y로 선언
    }
    else
    {
      rbtree_transplant(t, y, y->right); // z에 z의 successor인 y를 이식하기 전에, y의 자리에 y의 자식을 이식 시켜둔다. (그렇지 않으면, y는 자식이 달려 있는 상태로 z에 이식되기 때문이다)
      y->right = z->right;               // z 자리에 y를 이식하기 전, y의 오른쪽 자식을 z의 오른쪽 자식으로 교체
      y->right->parent = y;              // z자리에 y를 이식하기 전, y의 오른쪽 자식의 부모 노드를 y로 교체
    }

    rbtree_transplant(t, z, y); // z의 자리에 y를 이식한다.
    y->left = z->left;          // z의 왼쪽 자식이 y의 왼쪽 자식이 된다.
    y->left->parent = y;        // z의 왼쪽 자식의 부모가 곧, y가 된다.
    y->color = z->color;        // y가 z의 자리에 이식되고, y는 z의 색깔만 물려 받는다(값은 본래 y의 값 유지)
  }
  // 만약 삭제 대상의 색깔이 BLACK이라면, 각 경로의 BLACK 노드의 갯수의 균형이 깨지게 되므로, 균형을 맞춰 주어야 한다.
  if (y_original_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, x);
  }

  free(z); // z가 삭제 되었으므로, z의 메모리 주소를 비워준다.
  t->nil->parent = NULL;
  t->nil->right = NULL;
  t->nil->left = NULL;
  t->nil->color = RBTREE_BLACK;
  return 0;
}

void sort_asc(node_t *root, node_t *nil, key_t *arr, int *index) {

  if (root == nil) {
    return;
  }
  sort_asc(root->left, nil, arr, index);
  arr[*index] = root->key;
  (*index)++;
  sort_asc(root->right, nil, arr, index);
}

// rbtree 내용을 key 순서대로 주어진 array로 변환
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  int *index = calloc(1, sizeof(int));
  sort_asc(t->root, t->nil, arr, index);
  free(index);
  return 0;
}
