
// Code for managing linked lists. Nodes are allocated from a fixed-size pool.

#pragma once


// header for list nodes
typedef struct LIST_NODE_HDR
{
    // pointer to next node in the list
    struct LIST_NODE_HDR *next;
    // is the node free?
    BOOL free;
} LIST_NODE_HDR;


/// <summary>
/// Allocates a new list node.
/// </summary>
/// <param name="pool">Pointer to pool memory</param>
/// <param name="elemSize">Size, in bytes, of a list element</param>
/// <param name="poolSize">Size, in bytes, of the entire pool</param>
/// <returns>Pointer to new list node, or NULL</returns>
EXTERN_C LIST_NODE_HDR *AllocListNode(
    void *pool,
    size_t elemSize,
    size_t poolSize
);
PATCH_CODE(0x4489c0, 0x448850, AllocListNode);

/// <summary>
/// Initializes a pool used to allocate list nodes.
/// </summary>
/// <param name="pool">Pointer to pool memory</param>
/// <param name="elemSize">Size, in bytes, of a list element</param>
/// <param name="poolSize">Size, in bytes, of the entire pool</param>
EXTERN_C void InitListPool(void *pool, size_t elemSize, size_t poolSize);
PATCH_CODE(0x448a00, 0x448890, InitListPool);

/// <summary>
/// Removes an element from a list and frees the node.
/// </summary>
/// <param name="list">List to modify</param>
/// <param name="elem">Element to remove</param>
/// <returns>New list head</returns>
EXTERN_C LIST_NODE_HDR *RemoveListNode(
    LIST_NODE_HDR *list,
    LIST_NODE_HDR *elem
);
PATCH_CODE(0x448a40, 0x4488d0, RemoveListNode);

/// <summary>
/// Removes and frees all nodes from a list.
/// </summary>
/// <param name="list">List to clear</param>
/// <returns>New list head, which is always null</returns>
EXTERN_C LIST_NODE_HDR *ClearList(LIST_NODE_HDR *list);
PATCH_CODE(0x448a90, 0x448920, ClearList);

/// <summary>
/// Appends a node to the end of a list.
/// </summary>
/// <param name="list">List to append to</param>
/// <param name="elem">Element to append</param>
/// <returns>New list head</returns>
EXTERN_C LIST_NODE_HDR *AppendListNode(
    LIST_NODE_HDR *list,
    LIST_NODE_HDR *elem
);
PATCH_CODE(0x448ac0, 0x448950, AppendListNode);
