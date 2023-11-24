#include <stdio.h>
#include <stdlib.h>

typedef enum { LeafTag, NodeTag } TreeTag;

typedef struct Tree {
  TreeTag tag;
  union {
    unsigned int x;
    struct {
      struct Tree* x0;
      struct Tree* x1;
    };
  };
} Tree;

Tree* Leaf(unsigned int x) {
  Tree* t = malloc(sizeof(Tree));
  t->tag = LeafTag;
  t->x = x;
  return t;
}

Tree* Node(Tree* x0, Tree* x1) {
  Tree* t = malloc(sizeof(Tree));
  t->tag = NodeTag;
  t->x0 = x0;
  t->x1 = x1;
  return t;
}

unsigned int add(unsigned int a, unsigned int b) {
  return a + b;
}

Tree* gen(unsigned int n) {
  if (n == 0) {
    return Leaf(1);
  } else {
    unsigned int p = n - 1;
    return Node(gen(p), gen(p));
  }
}

unsigned int sum(Tree* t) {
  if (t->tag == LeafTag) {
    return t->x;
  } else {
    return add(sum(t->x0), sum(t->x1));
  }
}

int main() {
  printf("%u\n", sum(gen(24)));
  return 0;
}
