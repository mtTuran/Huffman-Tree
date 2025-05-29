#include <stdio.h>
#include <stdlib.h>
#include "../include/huffman.h"

int height(node* root) {
    if (root == NULL)
        return 0;
    else {
        int left_height = height(root->left);
        int right_height = height(root->right);
        
        return (left_height > right_height) ? (left_height + 1) : (right_height + 1);
    }
}

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

void alt_insert(queue** head, node* new_element){   // will be used by the decoder. for this function recurrence means bit length
    queue* new_node = (queue*) malloc(sizeof(queue));
    new_node -> element = new_element;
    if (*head == NULL){       
        new_node -> next_element = NULL;
        *head = new_node;
        return;
    }

    queue* curr = *head;
    queue* prev = NULL;
    while (curr != NULL && curr -> element -> recurrence <= new_node -> element -> recurrence)  // <= otherwise same length letters in decoder
    {                                                                                           // will have a reversed bit
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

void print_encoding(node* root, int coding[], int top){
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL){
        printf("%c: ", root -> letter);
        for (int i = 0; i < top; i = i + 1)
        {
            printf("%d", coding[i]);
        }
        printf("\n");
    }

    if (root->left){
        coding[top] = 0;
        print_encoding(root -> left, coding, top + 1);
    }

    if (root->right){
        coding[top] = 1;
        print_encoding(root -> right, coding, top + 1);
    }

}