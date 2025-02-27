#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an element which value is s */
element_t *new_ele(char *s)
{
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return NULL;

    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return NULL;
    }

    INIT_LIST_HEAD(&new->list);
    return new;
}

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *entry = NULL, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        q_release_element(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new = new_ele(s);
    if (!new)
        return false;
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new = new_ele(s);
    if (!new)
        return false;
    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *first = list_first_entry(head, element_t, list);
    if (sp && first->value) {
        strncpy(sp, first->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_del(&first->list);
    return first;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *last = list_last_entry(head, element_t, list);
    if (sp && last->value) {
        strncpy(sp, last->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_del(&last->list);
    return last;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *slow, *fast = head;
    list_for_each (slow, head)
        if (fast->next != head && fast->next->next != head)
            fast = fast->next->next;
        else
            break;
    list_del(slow);
    q_release_element(list_entry(slow, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *node, *safe, *last_same = head;
    list_for_each_safe (node, safe, head)
        if (node->next == head ||
            strcmp(list_entry(node, element_t, list)->value,
                   list_entry(node->next, element_t, list)->value)) {
            struct list_head *tmp = malloc(sizeof(struct list_head));
            if (node->prev != head &&
                strcmp(list_entry(node, element_t, list)->value,
                       list_entry(node->prev, element_t, list)->value) == 0) {
                list_cut_position(tmp, last_same, node);
                q_free(tmp);
            } else
                last_same = node;
        }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *node;
    list_for_each (node, head)
        if (node->next != head) {
            element_t *first = list_entry(node, element_t, list);
            element_t *second = list_entry(node->next, element_t, list);
            char *tmp = first->value;
            first->value = second->value;
            second->value = tmp;
            node = node->next;
        }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head)
        list_move(node, head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;

    struct list_head *from = head, *to, *safe, tmp, *ptmp = &tmp;
    int i = 0;
    list_for_each_safe (to, safe, head)
        if (++i == k) {
            INIT_LIST_HEAD(ptmp);
            list_cut_position(ptmp, from, to);
            q_reverse(ptmp);
            list_splice(ptmp, from);
            i = 0;
            from = safe->prev;
        }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
