#include "llist.h"

linklist create_null_list_link(void)
{
    linklist llist = (linklist)malloc(sizeof(struct node));
    if (llist != NULL)
        llist->next = NULL;
    else
        fprintf(stderr, "out of space!\n");

    return llist;
}

int is_nulllist_link(linklist llist)
{
    return llist->next == NULL;
}

linklist insert_link(linklist llist, void *packet)
{
    pnode q = (pnode)malloc(sizeof(struct node));
    if (q == NULL) {
        fprintf(stderr, "out of space!\n");
    } else {
        q->rtp = packet;
        q->next = llist->next;
        llist->next = q;
    }
    return llist;
}

linklist delete_node(linklist llist, void *packet)
{
    if (llist->next == NULL)
        return llist;

    pnode q = llist->next;
    pnode p = llist;
    do {
        if (q->rtp == packet) {
            p->next = q->next;
            free(q->rtp);
            free(q);
            q = NULL;
            break;
        }
        p = q;
        q = q->next;
    } while (q != NULL);

    return llist;
}


pnode delete_this_node(linklist llist, pnode this_pnode)
{
    if (llist->next == NULL)
        return llist;

    pnode q = llist->next;
    pnode p = llist;

    do {
        if (q == this_pnode) {
            p->next = q->next;
            free(q->rtp);
            free(q);
            q = NULL;
            break;
        }
        p = q;
        q = q->next;
    } while (q != NULL);

    return p;
}

/*
 * return 0 if no find; else return 1.
 */
pnode search_node(linklist llist, void *packet)
{
    pnode q = llist->next;

    if (is_nulllist_link(llist))
        return NULL;

    do {
        if (q->rtp == packet)
            return q;

        q = q->next;
    } while (q != NULL);

    return NULL;
}

int num_node(linklist llist)
{
    int n = 0;
    while (llist->next) {
        n++;
        llist = llist->next;
    }
    return n;
}

/*
 * return 0: fail
 * return 1: succeed
 */
int insert_nodulp_node(linklist llist, void *packet)
{
    int ret = 0;
    if (search_node(llist, packet) == NULL) {
        insert_link(llist, packet);
        ret = 1;
    }

    return ret;
}

/*
 * return 0: empty linklist
 */
int free_linklist(linklist llist)
{
    if (llist->next == NULL)
        return 0;

    pnode q = llist->next->next; 
    do {
    	free(llist->next->rtp);
        free(llist->next);
        llist->next = q;
        q = q->next;
    } while (q != NULL);
    return 1;
}

