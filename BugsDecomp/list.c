
#include "stdafx.h"
#include "common.h"
#include "list.h"


LIST_NODE_HDR *AllocListNode(void *pool, size_t elemSize, size_t poolSize)
{
    // search the list for a free node
    LIST_NODE_HDR *n = pool;
    LIST_NODE_HDR *end = (LIST_NODE_HDR *)((char *)pool + poolSize);
    while (n < end)
    {
        // XXX: this is what the game code does
        if (n->free & 1)
        {
            n->free = FALSE;
            n->next = NULL;
            return n;
        }

        // move to next
        n = (LIST_NODE_HDR *)((char *)n + elemSize);
    }

    // no dice
    return NULL;
}

void InitListPool(void *pool, size_t elemSize, size_t poolSize)
{
    LIST_NODE_HDR *n = pool;
    LIST_NODE_HDR *end = (LIST_NODE_HDR *)((char *)pool + poolSize);
    while (n < end)
    {
        // zero the entire node
        memset(n, 0, elemSize);
        // mark node as free
        n->free = TRUE;
        // move to next
        n = (LIST_NODE_HDR *)((char *)n + elemSize);
    }
}

LIST_NODE_HDR *RemoveListNode(LIST_NODE_HDR *list, LIST_NODE_HDR *elem)
{
    // if list is empty, exit now
    if (!list)
        return NULL;

    // check if removing the list head
    if (list == elem)
    {
        // free element's node
        elem->free = TRUE;
        // next element becomes the list head
        return list->next;
    }

    // element is not at the head; search for previous node
    LIST_NODE_HDR *prev = list;
    while (prev && prev->next != elem)
        prev = prev->next;

    // unlink and free the node (if found)
    if (prev)
    {
        prev->next = elem->next;
        elem->next = NULL;
        elem->free = TRUE;
    }

    // list head is still intact
    return list;
}

LIST_NODE_HDR *ClearList(LIST_NODE_HDR *list)
{
    // walk through list and clear each node
    while (list)
    {
        LIST_NODE_HDR *next = list->next;
        list->free = TRUE;
        list->next = NULL;
        list = next;
    }

    return NULL;
}

LIST_NODE_HDR *AppendListNode(LIST_NODE_HDR *list, LIST_NODE_HDR *elem)
{
    // if list is empty, new node becomes the list
    if (!list)
        return elem;

    // find last node
    LIST_NODE_HDR *last = list;
    while (last->next)
        last = last->next;

    // link in new node
    last->next = elem;
    elem->next = NULL;

    return list;
}
