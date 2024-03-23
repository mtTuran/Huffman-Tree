#ifndef HUFFMANFUN_H
#define HUFFMANFUN_H

typedef struct node
{
    int recurrence;
    char letter;
    struct node* left;
    struct node* right;
}node;

typedef struct queue{
    node* element;
    struct queue* next_element;
}queue;

void insert_to_queue(queue** head, node* new_element);
node* pop(queue** head);
node* create_leaf(int recurrence, char letter);
node* huffman(node* small, node* large);

#endif