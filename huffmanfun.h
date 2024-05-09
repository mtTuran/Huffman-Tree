#ifndef HUFFMANFUN_H
#define HUFFMANFUN_H

typedef struct node
{
    int recurrence; // will be used as encoding length for canonical format as well
    char letter;
    struct node* left;
    struct node* right;
}node;

typedef struct queue{
    node* element;
    struct queue* next_element;
}queue;

int height(node* root);
void insert_to_queue(queue** head, node* new_element);
void alt_insert(queue** head, node* new_element);
node* pop(queue** head);
node* create_leaf(int recurrence, char letter);
node* huffman(node* small, node* large);
void print_encoding(node* root, int coding[], int top);

#endif