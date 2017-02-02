#include "llist.h"
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
linklist create_null_list_link(void)
{
	pthread_mutex_lock(&lock);
    linklist llist = (linklist)malloc(sizeof(struct node));
    if (llist != NULL)
        llist->next = NULL;
    else
        fprintf(stderr, "out of space!\n");
	pthread_mutex_unlock(&lock);

    return llist;
}

int is_nulllist_link(linklist llist)
{
    return llist->next == NULL;
}

linklist insert_link(linklist llist, void *packet)
{
	pthread_mutex_lock(&lock);
    pnode q = (pnode)malloc(sizeof(struct node));
    if (q == NULL) {
        fprintf(stderr, "out of space!\n");
    } else {
        q->data = packet;
        q->next = llist->next;
        llist->next = q;
    }
	pthread_mutex_unlock(&lock);
    return llist;
}

linklist delete_node(linklist llist, void *packet)
{
    if (llist->next == NULL)
        return llist;

	pthread_mutex_lock(&lock);
    pnode q = llist->next;
    pnode p = llist;
    do {
        if (q->data == packet) {
            p->next = q->next;
            free(q->data);
            free(q);
            q = NULL;
            break;
        }
        p = q;
        q = q->next;
    } while (q != NULL);
	pthread_mutex_unlock(&lock);

    return llist;
}


pnode delete_this_node(linklist llist, pnode this_pnode)
{
    if (llist->next == NULL)
        return llist;

	pthread_mutex_lock(&lock);
    pnode q = llist->next;
    pnode p = llist;

    do {
        if (q == this_pnode) {
            p->next = q->next;
            free(q->data);
            free(q);
            q = NULL;
            break;
        }
        p = q;
        q = q->next;
    } while (q != NULL);
	pthread_mutex_unlock(&lock);

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

	pthread_mutex_lock(&lock);
    do {
        if (q->data == packet)
		{
			pthread_mutex_unlock(&lock);
            return q;
		}

        q = q->next;
    } while (q != NULL);
	pthread_mutex_unlock(&lock);

    return NULL;
}

int num_node(linklist llist)
{
    int n = 0;
	pthread_mutex_lock(&lock);
    while (llist->next) {
        n++;
        llist = llist->next;
    }
	pthread_mutex_unlock(&lock);
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
    	free(llist->next->data);
        free(llist->next);
        llist->next = q;
        q = q->next;
    } while (q != NULL);
	pthread_mutex_unlock(&lock);
    return 1;
}

