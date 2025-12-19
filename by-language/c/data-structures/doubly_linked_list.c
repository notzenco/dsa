/**
 * Doubly Linked List Implementation
 * See doubly_linked_list.h for documentation
 */

#include "doubly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

/* Internal: Create a new node */
static DLLNode *dll_create_node(int value) {
    DLLNode *node = malloc(sizeof(DLLNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

DoublyLinkedList *dll_create(void) {
    DoublyLinkedList *list = malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void dll_destroy(DoublyLinkedList *list) {
    if (list == NULL) {
        return;
    }
    dll_clear(list);
    free(list);
}

size_t dll_size(const DoublyLinkedList *list) {
    return list != NULL ? list->size : 0;
}

bool dll_is_empty(const DoublyLinkedList *list) {
    return list == NULL || list->size == 0;
}

bool dll_push_front(DoublyLinkedList *list, int value) {
    if (list == NULL) {
        return false;
    }

    DLLNode *node = dll_create_node(value);
    if (node == NULL) {
        return false;
    }

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->size++;
    return true;
}

bool dll_push_back(DoublyLinkedList *list, int value) {
    if (list == NULL) {
        return false;
    }

    DLLNode *node = dll_create_node(value);
    if (node == NULL) {
        return false;
    }

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return true;
}

bool dll_insert(DoublyLinkedList *list, size_t index, int value) {
    if (list == NULL || index > list->size) {
        return false;
    }

    if (index == 0) {
        return dll_push_front(list, value);
    }

    if (index == list->size) {
        return dll_push_back(list, value);
    }

    DLLNode *node = dll_create_node(value);
    if (node == NULL) {
        return false;
    }

    /* Find the node at the target index */
    DLLNode *current;
    if (index <= list->size / 2) {
        /* Search from head */
        current = list->head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        /* Search from tail */
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }

    /* Insert before current */
    node->next = current;
    node->prev = current->prev;
    current->prev->next = node;
    current->prev = node;

    list->size++;
    return true;
}

bool dll_pop_front(DoublyLinkedList *list, int *out_value) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    DLLNode *node = list->head;
    if (out_value != NULL) {
        *out_value = node->data;
    }

    list->head = node->next;
    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(node);
    list->size--;
    return true;
}

bool dll_pop_back(DoublyLinkedList *list, int *out_value) {
    if (list == NULL || list->tail == NULL) {
        return false;
    }

    DLLNode *node = list->tail;
    if (out_value != NULL) {
        *out_value = node->data;
    }

    list->tail = node->prev;
    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(node);
    list->size--;
    return true;
}

bool dll_remove(DoublyLinkedList *list, size_t index, int *out_value) {
    if (list == NULL || index >= list->size) {
        return false;
    }

    if (index == 0) {
        return dll_pop_front(list, out_value);
    }

    if (index == list->size - 1) {
        return dll_pop_back(list, out_value);
    }

    DLLNode *node = dll_get_node(list, index);
    return dll_remove_node(list, node, out_value);
}

bool dll_remove_node(DoublyLinkedList *list, DLLNode *node, int *out_value) {
    if (list == NULL || node == NULL) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = node->data;
    }

    /* Handle head */
    if (node == list->head) {
        return dll_pop_front(list, NULL);
    }

    /* Handle tail */
    if (node == list->tail) {
        return dll_pop_back(list, NULL);
    }

    /* Middle node */
    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node);
    list->size--;
    return true;
}

bool dll_remove_value(DoublyLinkedList *list, int value) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    DLLNode *current = list->head;
    while (current != NULL) {
        if (current->data == value) {
            return dll_remove_node(list, current, NULL);
        }
        current = current->next;
    }

    return false;
}

bool dll_peek_front(const DoublyLinkedList *list, int *out_value) {
    if (list == NULL || list->head == NULL || out_value == NULL) {
        return false;
    }
    *out_value = list->head->data;
    return true;
}

bool dll_peek_back(const DoublyLinkedList *list, int *out_value) {
    if (list == NULL || list->tail == NULL || out_value == NULL) {
        return false;
    }
    *out_value = list->tail->data;
    return true;
}

bool dll_get(const DoublyLinkedList *list, size_t index, int *out_value) {
    if (list == NULL || index >= list->size || out_value == NULL) {
        return false;
    }

    DLLNode *node = dll_get_node(list, index);
    if (node == NULL) {
        return false;
    }

    *out_value = node->data;
    return true;
}

bool dll_set(DoublyLinkedList *list, size_t index, int value) {
    if (list == NULL || index >= list->size) {
        return false;
    }

    DLLNode *node = dll_get_node(list, index);
    if (node == NULL) {
        return false;
    }

    node->data = value;
    return true;
}

DLLNode *dll_get_node(const DoublyLinkedList *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    DLLNode *current;
    if (index <= list->size / 2) {
        /* Search from head */
        current = list->head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        /* Search from tail */
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }

    return current;
}

int dll_find(const DoublyLinkedList *list, int value) {
    if (list == NULL) {
        return -1;
    }

    DLLNode *current = list->head;
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

bool dll_contains(const DoublyLinkedList *list, int value) {
    return dll_find(list, value) != -1;
}

void dll_clear(DoublyLinkedList *list) {
    if (list == NULL) {
        return;
    }

    DLLNode *current = list->head;
    while (current != NULL) {
        DLLNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void dll_reverse(DoublyLinkedList *list) {
    if (list == NULL || list->size <= 1) {
        return;
    }

    DLLNode *current = list->head;
    DLLNode *temp;

    /* Swap next and prev for all nodes */
    while (current != NULL) {
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = temp;
    }

    /* Swap head and tail */
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

void dll_move_to_front(DoublyLinkedList *list, DLLNode *node) {
    if (list == NULL || node == NULL || node == list->head) {
        return;
    }

    /* Remove node from current position */
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }

    /* Insert at front */
    node->prev = NULL;
    node->next = list->head;
    if (list->head != NULL) {
        list->head->prev = node;
    }
    list->head = node;
}

void dll_move_to_back(DoublyLinkedList *list, DLLNode *node) {
    if (list == NULL || node == NULL || node == list->tail) {
        return;
    }

    /* Remove node from current position */
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node == list->head) {
        list->head = node->next;
    }

    /* Insert at back */
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail != NULL) {
        list->tail->next = node;
    }
    list->tail = node;
}

void dll_print_forward(const DoublyLinkedList *list) {
    if (list == NULL) {
        printf("NULL\n");
        return;
    }

    printf("HEAD <-> ");
    DLLNode *current = list->head;
    while (current != NULL) {
        printf("[%d] <-> ", current->data);
        current = current->next;
    }
    printf("TAIL (size=%lu)\n", (unsigned long)list->size);
}

void dll_print_backward(const DoublyLinkedList *list) {
    if (list == NULL) {
        printf("NULL\n");
        return;
    }

    printf("TAIL <-> ");
    DLLNode *current = list->tail;
    while (current != NULL) {
        printf("[%d] <-> ", current->data);
        current = current->prev;
    }
    printf("HEAD (size=%lu)\n", (unsigned long)list->size);
}
