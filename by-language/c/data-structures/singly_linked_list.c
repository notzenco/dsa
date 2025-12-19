/**
 * Singly Linked List Implementation
 * See singly_linked_list.h for documentation
 */

#include "singly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

/* Internal: Create a new node */
static SLLNode *sll_create_node(int value) {
    SLLNode *node = malloc(sizeof(SLLNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = value;
    node->next = NULL;
    return node;
}

SinglyLinkedList *sll_create(void) {
    SinglyLinkedList *list = malloc(sizeof(SinglyLinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void sll_destroy(SinglyLinkedList *list) {
    if (list == NULL) {
        return;
    }
    sll_clear(list);
    free(list);
}

size_t sll_size(const SinglyLinkedList *list) {
    return list != NULL ? list->size : 0;
}

bool sll_is_empty(const SinglyLinkedList *list) {
    return list == NULL || list->size == 0;
}

bool sll_push_front(SinglyLinkedList *list, int value) {
    if (list == NULL) {
        return false;
    }

    SLLNode *node = sll_create_node(value);
    if (node == NULL) {
        return false;
    }

    node->next = list->head;
    list->head = node;

    if (list->tail == NULL) {
        list->tail = node;
    }

    list->size++;
    return true;
}

bool sll_push_back(SinglyLinkedList *list, int value) {
    if (list == NULL) {
        return false;
    }

    SLLNode *node = sll_create_node(value);
    if (node == NULL) {
        return false;
    }

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return true;
}

bool sll_insert(SinglyLinkedList *list, size_t index, int value) {
    if (list == NULL || index > list->size) {
        return false;
    }

    if (index == 0) {
        return sll_push_front(list, value);
    }

    if (index == list->size) {
        return sll_push_back(list, value);
    }

    SLLNode *node = sll_create_node(value);
    if (node == NULL) {
        return false;
    }

    SLLNode *current = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    list->size++;
    return true;
}

bool sll_pop_front(SinglyLinkedList *list, int *out_value) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    SLLNode *node = list->head;
    if (out_value != NULL) {
        *out_value = node->data;
    }

    list->head = node->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }

    free(node);
    list->size--;
    return true;
}

bool sll_pop_back(SinglyLinkedList *list, int *out_value) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    if (list->head == list->tail) {
        return sll_pop_front(list, out_value);
    }

    /* Find second to last node */
    SLLNode *current = list->head;
    while (current->next != list->tail) {
        current = current->next;
    }

    if (out_value != NULL) {
        *out_value = list->tail->data;
    }

    free(list->tail);
    current->next = NULL;
    list->tail = current;
    list->size--;
    return true;
}

bool sll_remove(SinglyLinkedList *list, size_t index, int *out_value) {
    if (list == NULL || index >= list->size) {
        return false;
    }

    if (index == 0) {
        return sll_pop_front(list, out_value);
    }

    SLLNode *prev = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        prev = prev->next;
    }

    SLLNode *node = prev->next;
    if (out_value != NULL) {
        *out_value = node->data;
    }

    prev->next = node->next;
    if (node == list->tail) {
        list->tail = prev;
    }

    free(node);
    list->size--;
    return true;
}

bool sll_remove_value(SinglyLinkedList *list, int value) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    if (list->head->data == value) {
        return sll_pop_front(list, NULL);
    }

    SLLNode *prev = list->head;
    SLLNode *current = list->head->next;

    while (current != NULL) {
        if (current->data == value) {
            prev->next = current->next;
            if (current == list->tail) {
                list->tail = prev;
            }
            free(current);
            list->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

bool sll_peek_front(const SinglyLinkedList *list, int *out_value) {
    if (list == NULL || list->head == NULL || out_value == NULL) {
        return false;
    }
    *out_value = list->head->data;
    return true;
}

bool sll_peek_back(const SinglyLinkedList *list, int *out_value) {
    if (list == NULL || list->tail == NULL || out_value == NULL) {
        return false;
    }
    *out_value = list->tail->data;
    return true;
}

bool sll_get(const SinglyLinkedList *list, size_t index, int *out_value) {
    if (list == NULL || index >= list->size || out_value == NULL) {
        return false;
    }

    SLLNode *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    *out_value = current->data;
    return true;
}

bool sll_set(SinglyLinkedList *list, size_t index, int value) {
    if (list == NULL || index >= list->size) {
        return false;
    }

    SLLNode *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    current->data = value;
    return true;
}

int sll_find(const SinglyLinkedList *list, int value) {
    if (list == NULL) {
        return -1;
    }

    SLLNode *current = list->head;
    int index = 0;

    while (current != NULL) {
        if (current->data == value) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

bool sll_contains(const SinglyLinkedList *list, int value) {
    return sll_find(list, value) != -1;
}

void sll_clear(SinglyLinkedList *list) {
    if (list == NULL) {
        return;
    }

    SLLNode *current = list->head;
    while (current != NULL) {
        SLLNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void sll_reverse(SinglyLinkedList *list) {
    if (list == NULL || list->size <= 1) {
        return;
    }

    SLLNode *prev = NULL;
    SLLNode *current = list->head;
    list->tail = list->head;

    while (current != NULL) {
        SLLNode *next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    list->head = prev;
}

bool sll_get_middle(const SinglyLinkedList *list, int *out_value) {
    if (list == NULL || list->head == NULL || out_value == NULL) {
        return false;
    }

    /* Floyd's tortoise and hare approach */
    SLLNode *slow = list->head;
    SLLNode *fast = list->head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    *out_value = slow->data;
    return true;
}

bool sll_has_cycle(const SinglyLinkedList *list) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    /* Floyd's cycle detection */
    SLLNode *slow = list->head;
    SLLNode *fast = list->head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }

    return false;
}

bool sll_to_array(const SinglyLinkedList *list, int **out_array, size_t *out_size) {
    if (list == NULL || out_array == NULL || out_size == NULL) {
        return false;
    }

    if (list->size == 0) {
        *out_array = NULL;
        *out_size = 0;
        return true;
    }

    *out_array = malloc(list->size * sizeof(int));
    if (*out_array == NULL) {
        return false;
    }

    SLLNode *current = list->head;
    size_t i = 0;
    while (current != NULL) {
        (*out_array)[i++] = current->data;
        current = current->next;
    }

    *out_size = list->size;
    return true;
}

void sll_print(const SinglyLinkedList *list) {
    if (list == NULL) {
        printf("NULL\n");
        return;
    }

    printf("HEAD -> ");
    SLLNode *current = list->head;
    while (current != NULL) {
        printf("[%d] -> ", current->data);
        current = current->next;
    }
    printf("NULL (size=%lu)\n", (unsigned long)list->size);
}
