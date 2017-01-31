#ifndef _LLIST_H
#define _LLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef MAX_SEND_FAIL_N
#define MAX_SEND_FAIL_N     29
#endif

struct node {
    void *rtp;
    struct node *next;
};
typedef struct node *pnode;
typedef struct node *linklist;

linklist create_null_list_link(void);
int is_nulllist_link(linklist llist);
linklist insert_link(linklist llist, void *packet);
linklist delete_node(linklist llist, void *packet);

pnode delete_this_node(linklist llist, pnode this_pnode);

pnode search_node(linklist llist, void *packet);
int num_node(linklist llist);
int insert_nodulp_node(linklist llist, void *packet);
int free_linklist(linklist llist);

#endif
