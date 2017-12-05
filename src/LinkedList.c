#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "LinkedList.h"

/**
 * Represents a linked list
 * @param  size size of the linked list
 * @param  head first item in the linked list
 * @param  tail last item in the linked list
 */
struct LinkedList {
    int size;
    struct Node* head;
    struct Node* tail;
};

/**
 * reprensts a node in a linked list
 * @param  data     data the node holds
 * @param  next     next node in the linked list
 * @param  previous previous node in the linked list
 */
struct Node {
    struct Node* next;
    struct Node* previous;
    void* data;
};

/**
 * creates a pointer to a new node
 * @param  data data the node holds
 * @return      a pointer to the newly created node
 */
Node* new_Node(void* data) {
    Node* opt = malloc(sizeof(Node));
    if (opt == NULL) {
        puts("node malloc error!");
        return NULL;
    }
    opt->data = data;
    opt->next = NULL;
    opt->previous = NULL;
    return opt;
}

/**
 * creates a pointer to a new linked list
 * @return  a pointer to a newly created linked list
 */
LinkedList* new_list(void) {
    LinkedList* opt = malloc(sizeof(LinkedList));
    if (opt == NULL) {
        puts("list malloc error!");
        return NULL;
    }
    opt->head = NULL;
    opt->tail = NULL;
    opt->size = 0;
    return opt;
}

/**
 * add to the end of a linked list
 * @param  l    a pointer to the linked list of interest
 * @param  data the data a new node will hold
 * @return      whether successful
 */
int add_list(LinkedList* l, void* data) {
    if (l == NULL) {
        return 1;
    }
    Node* added = new_Node(data);
    if (l->size == 0) {
        l->head = added;
        l->tail = added;
    } else {
        l->tail->next = added;
        added->previous = l->tail;
        l->tail = added;
    }
    l->size++;
    return 0;
}

/**
 * removes an item from the end of the linked list
 * @param l a pointer to the linked list of interest
 * @return  the data the removed node holds
 */
void* pop(LinkedList* l) {
    if (l == NULL) {
        puts("pop NULL ptr");
        return NULL;
    }
    if (l->size == 0) {
        puts("no element");
        return NULL;
    }
    if (l->size == 1) {
        void* opt = l->tail->data;
        free(l->tail);
        l->head = NULL;
        l->tail = NULL;
        l->size--;
        return opt;
    }
    void* opt = l->tail->data;
    Node* nt = l->tail->previous;
    free(l->tail);
    l->tail = nt;
    l->tail->next = NULL;
    l->size--;
    return opt;
}

/**
 * removes an item from the head of the linked list
 * @param l a pointer to the linked list of interest
 * @return  the data the removed node holds
 */
void* dequeue(LinkedList* l) {
    if (l == NULL) {
        puts("dequeue NULL ptr");
        return NULL;
    }
    if (l->size == 0) {
        puts("no element");
        return NULL;
    }
    if (l->size == 1) {
        void* opt = l->head->data;
        free(l->head);
        l->head = NULL;
        l->tail = NULL;
        l->size--;
        return opt;
    }
    void* opt = l->head->data;
    void* nh = l->head->next;
    free(l->head);
    l->head = nh;
    l->head->previous = NULL;
    l->size--;
    return opt;
}

/**
 * gets the data an arbitrary node in the list holds
 * @param l   a pointer to the linked list of interest
 * @param ind index of the node in the linked list
 * @return    the data the node of interest holds
 */
void* get(LinkedList* l, int ind) {
    int tind;
    Node* temp;
    int le;
    if (l == NULL) {
        puts("get NULL ptr");
        return NULL;
    }
    if (ind < 0 || ind >= l->size) {
        puts("get index out of bound");
        return NULL;
    }
    //if index is less thqn half the size, go from head; oterwise, go from tail
    if (ind < l->size / 2) {
        tind = 0;
        temp = l->head;
        le = 1;
    } else {
        tind = l->size - 1;
        temp = l->tail;
        le = 0;
    }
    while (tind != ind) {
        if (le) {
            temp = temp->next;
            tind++;
        } else {
            temp = temp->previous;
            tind--;
        }
    }
    return temp->data;
}

/**
 * removes an arbitrary node from a linked list
 * @param  l   a pointer to the linked list of interest
 * @param  ind the index of the node to be removed
 * @return     whether successful
 */
int remove_ind(LinkedList* l, int ind) {
    if (l == NULL) {
        puts("remove NULL ptr");
        return 1;
    }
    if (ind < 0 || ind >= l->size) {
        puts("remove index out of bound");
        return 1;
    }
    //call dequeue if index is zero
    if (ind == 0) {
        dequeue(l);
        return 0;
    }
    //call pop if index is size - 1
    if (ind == l->size - 1) {
        pop(l);
        return 0;
    }
    int tind;
    Node* temp;
    int le;
    if (ind < l->size / 2) {
        tind = 0;
        temp = l->head;
        le = 1;
    } else {
        tind = l->size - 1;
        temp = l->tail;
        le = 0;
    }
    while (tind != ind) {
        if (le) {
            temp = temp->next;
            tind++;
        } else {
            temp = temp->previous;
            tind--;
        }
    }
    temp->previous->next = temp->next;
    temp->next->previous = temp->previous;
    free(temp);
    l->size--;
    return 0;
}

/**
 * gets the size of a linked list
 * @param  l   a pointer to the linked list of interest
 * @param  opt a pointer to the variable the result will be put in
 * @return     whether successful
 */
int get_size(LinkedList* l, int* opt) {
    if (l == NULL) {
        puts("get size NULL ptr");
        return 1;
    }
    *opt = l->size;
    return 0;
}

/**
 * frees the memory occupied by using a linked list pointer and pointers to all
 * its nodes
 * @param  l a pointer to the linked list of interest
 * @return   whether successful
 */
int free_list(LinkedList* l) {
    if (l == NULL) {
        puts("free list NULL ptr");
        return 1;
    }
    Node* temp = l->head;
    for (int i = 0; i < l->size; i++) {
        Node* temp1 = temp->next;
        free(temp);
        temp = temp1;
    }
    free(l);
    return 0;
}

/**
 * prints a linked list
 * @param  l a pointer to the linked list of interest
 * @return   whether successful
 */
int print_list(LinkedList* l) {
    if (l == NULL) {
        puts("free list NULL ptr");
        return 1;
    }
    Node* temp = l->head;
    for (int i = 0; i < l->size; i++) {
        //calls the handler function to be implemented to print out data
        handle_print(temp->data);
        temp = temp->next;
    }
    return 0;
}
