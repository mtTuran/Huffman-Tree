#include <stdio.h>
#include "huffmanfun.h"
#include "hashing.h"

int main(){
    queue* root = NULL;
    node* element = create_leaf(100, 'c');
    node* second = create_leaf(120, 'b');
    insert_to_queue(&root, element);
    insert_to_queue(&root, second);
    node* result = pop(&root);
    printf("%d %c", result->recurrence, result->letter);

    return 0;
}
