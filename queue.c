#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

#define DEBUG 1
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    list_ele_t *current;
    while (q->head != NULL) {
        current = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = current;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    int len = strlen(s) + 1;
    newh->value = (char *) malloc(sizeof(char) * len);
    if (newh->value) {
        free(newh);
        return false;
    }
    newh->value_len = len;
    memset(newh->value, 0x00, len);
    memcpy(newh->value, s, len);
    newh->next = NULL;
    /* What if either call to malloc returns NULL? */
    newh->next = q->head;
    q->head = newh;
    if (q->tail == NULL) {
        q->tail = q->head;
    }
    q->size = q->size + 1;
#if DEBUG
    printf("ih: %s\n", s);
#endif
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */

    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    int len = strlen(s) + 1;
    newh->value = (char *) malloc(sizeof(char) * len);
    if (!newh->value) {
        free(newh);
        return false;
    }
    newh->value_len = len;
    memset(newh->value, 0x00, len);
    memcpy(newh->value, s, len);
    newh->next = NULL;
    /* What if either call to malloc returns NULL? */
    if (q->tail == NULL) {
        q->tail = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->size = q->size + 1;
#if DEBUG
    printf("it: %s\n", s);
#endif
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL || q->head == NULL) {
        return false;
    }
    int copy_len = 0;
    list_ele_t *current = q->head;
    q->head = q->head->next;
    q->size = q->size - 1;
    copy_len = current->value_len >= bufsize ? bufsize - 1 : current->value_len;
    strncpy(sp, current->value, copy_len);
    if (copy_len == bufsize - 1) {
        sp[bufsize - 1] = '\0';
    }

    free(current->value);
    free(current);

#if DEBUG
    printf("sp: %s\n", sp);
#endif
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->head == NULL) {
        return;
    } else if (q->size == 1) {
        return;
    }

    list_ele_t *prev = q->head;
    list_ele_t *middle = q->head->next;
    q->head = q->head->next->next;
    q->tail = prev;
    q->tail->next = NULL;

    while (q->head != NULL) {
        middle->next = prev;
        prev = middle;
        middle = q->head;
        q->head = q->head->next;
    }
    middle->next = prev;
    q->head = middle;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q->size <= 1) {
        printf("no need sort\n");
        return;
    }
    list_ele_t *current;
    list_ele_t *round = q->head;
    char *tmp_addr = NULL;
    while (round->next != NULL) {
        current = round->next;
        while (current != NULL) {
            if (strcmp(current->value, round->value) < 0) {
                tmp_addr = current->value;
                current->value = round->value;
                round->value = tmp_addr;
            }
            current = current->next;
        }
        round = round->next;
    }

#if 0
    current = q->head;
    while (current != NULL) {
        printf("ih: %s\n", current->value);
        current = current->next;
    }
#endif
}
