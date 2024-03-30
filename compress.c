#include <stdio.h>
#include <stdlib.h>
#include "huffmanfun.h"
#include "hashing.h"

void hash_encode(hash_table* table, node* root, char bits[], int top){
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL){
        int index = search_index(table, root -> letter);
        encode_letter(table, root -> letter, bits, top);
    }

    if (root->left){
        bits[top] = 0;
        hash_encode(table, root -> left, bits, top + 1);
    }

    if (root->right){
        bits[top] = 1;
        hash_encode(table, root -> right, bits, top + 1);
    }
}

int main(){
    FILE* handle = fopen("mytext.txt", "r");

    if (handle != NULL){
        int char_limit = 255;
        char current;
        hash_table* dictionary = create_hash_table(char_limit);
        char* search_list = create_search_list(char_limit);
        while ((current = fgetc(handle)) != EOF)
        {
            map_to_table(dictionary, current);
            add_to_search_list(search_list, char_limit, current);
        }
        fclose(handle);

        queue* root = NULL;
        int number_of_nodes = 0;
        for (int i = 0; i < char_limit; i = i + 1){
            char c = search_list[i];
            if (c == '\0') break;
            else{
                insert_to_queue(&root, create_leaf(search_table(dictionary, c), c));
                number_of_nodes += 1;
            }
        }
        node* father = NULL;
        while (number_of_nodes > 1)
        {
            node* small = pop(&root);
            node* big = pop(&root);
            father = huffman(small, big);
            insert_to_queue(&root, father);
            number_of_nodes -= 1;
        }
        
        int max_bit = height(father);
        char* bits = (char*) malloc(sizeof(char) * (max_bit + 1));        
        hash_encode(dictionary, father, bits, 0);
        print_encode_table(dictionary);
        
    }else{
        printf("Error opening file");
        exit(1);
    }
    
    return 0;
}
