#include <stdlib.h>
#include "huffmanfun.h"

void insert_to_queue(queue** head, node* new_element){
    queue* new_node = (queue*) malloc(sizeof(queue));
    new_node -> element = new_element;
    if (*head == NULL){       
        new_node -> next_element = NULL;
        *head = new_node;
        return;
    }

    queue* curr = *head;
    queue* prev = NULL;
    while (curr != NULL && curr -> element -> recurrence < new_node -> element -> recurrence)
    {
        prev = curr;
        curr = curr -> next_element;   
    }
    if (prev == NULL) {
        new_node -> next_element = *head;
        *head = new_node;
    }
    else {
        prev -> next_element = new_node;
        new_node -> next_element = curr;
    } 
}

node* pop(queue** head){
    queue* popped = *head;
    queue* new_head = popped -> next_element;
    *head = new_head;
    node* element = popped -> element;
    free(popped);
    return element;
}

node* create_leaf(int recurrence, char letter){
    node* element = (node*) malloc(sizeof(node));
    element -> recurrence = recurrence;
    element -> letter = letter;
    element -> left = NULL;
    element -> right = NULL;
    return element;
}

node* huffman(node* small, node* large){
    node* father_node = (node*) malloc(sizeof(node));
    father_node -> left = small;
    father_node -> right = large;
    father_node -> recurrence = small -> recurrence + large -> recurrence;
    father_node -> letter = '\0';
    return father_node;
}