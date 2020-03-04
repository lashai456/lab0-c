#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

void print_data(queue_t *q)
{
#if DEBUG
    list_ele_t *current = q->head;
    while (current != NULL) {
        printf("%s,", current->value);
        current = current->next;
    }
    printf("\n");
#endif
}

int count_input_data(char *s)
{
    int len = 1;
    char *tmp = s;
    while (*tmp != '\0') {
        len = len + 1;
        tmp++;
    }
    return len;
}

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
#if DEBUG
    printf("%s\n", __func__);
#endif
    queue_t *q = malloc(sizeof(queue_t));
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
#if DEBUG
    printf("%s\n", __func__);
#endif
    if (!q) {
        printf("no need to free\n");
        return;
    }

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
#if DEBUG
    printf("%s,", __func__);
#endif
    list_ele_t *newh;
    int len = 0;

    if (!q) {
        goto exit;
    }

    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        goto exit;
    }

    len = count_input_data(s);
    if (len == 0) {
        goto insert_fail;
    }

    newh->value = (char *) malloc(sizeof(char) * len);
    if (!newh->value) {
        goto insert_fail;
    }

    newh->value_len = len;
    memset(newh->value, 0x00, len);
    memcpy(newh->value, s, len);
    newh->next = NULL;
    newh->next = q->head;
    q->head = newh;
    if (q->tail == NULL) {
        q->tail = q->head;
    }
    q->size = q->size + 1;
    print_data(q);
    return true;

insert_fail:
    free(newh);
exit:
    return false;
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
#if DEBUG
    printf("%s,", __func__);
#endif

    list_ele_t *newh;
    int len = 0;

    if (!q) {
        goto exit;
    }

    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        goto exit;
    }

    len = count_input_data(s);
    if (len == 0) {
        goto insert_fail;
    }

    newh->value = (char *) malloc(sizeof(char) * len);
    if (!newh->value) {
        goto insert_fail;
    }

    newh->value_len = len;
    memset(newh->value, 0x00, len);
    memcpy(newh->value, s, len);
    newh->next = NULL;
    if (q->tail == NULL) {
        q->tail = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->size = q->size + 1;
    print_data(q);
    return true;

insert_fail:
    free(newh);
exit:
    return false;
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
#if DEBUG
    printf("%s\n", __func__);
#endif

    if (q == NULL || q->size == 0 || bufsize == 0) {
        return false;
    }

    int copy_len = 0;
    list_ele_t *current = q->head;
    q->head = q->head->next;
    q->size = q->size - 1;
    copy_len = current->value_len >= bufsize ? bufsize : current->value_len;
    memset(sp, 0x00, bufsize);
    strncpy(sp, current->value, copy_len);
    sp[copy_len - 1] = '\0';

    free(current->value);
    free(current);
    print_data(q);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
#if DEBUG
    printf("%s\n", __func__);
#endif
    return q == NULL ? 0 : q->size;
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
#if DEBUG
    printf("%s\n", __func__);
#endif

    if (q == NULL || q->size <= 1) {
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
#if DEBUG
    printf("%s\n", __func__);
#endif
    if (q == NULL || q->size <= 1) {
        printf("no need sort\n");
        return;
    }
    q->head = merge_sort(q->head);
}

list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    if (!left) {
        return right;
    }

    if (!right) {
        return left;
    }

    if (strcmp(left->value, right->value) < 0) {
        left->next = merge(left->next, right);
        return left;
    } else {
        right->next = merge(left, right->next);
        return right;
    }
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || head->next == NULL) {
        return head;
    }

    list_ele_t *slow = head;
    list_ele_t *fast = head->next;
    list_ele_t *left;
    list_ele_t *right;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    fast = slow->next;
    slow->next = NULL;

    left = merge_sort(head);
    right = merge_sort(fast);

    return merge(left, right);
}
