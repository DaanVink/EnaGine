#ifndef ENA_RESOLVER_GLOBAL
#define ENA_RESOLVER_GLOBAL

struct node {
  char type[64];
  char resolved[64];
  struct node *next;
};
struct node *GLOBAL_ROOT;

#endif
