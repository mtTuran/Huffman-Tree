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

void encode_file(char* input_file, char* output_file, hash_table* table){
    FILE* input_handle = fopen(input_file, "rb");
    FILE* output_handle = fopen(output_file, "wb");

    if (!input_handle || !output_handle){
        printf("File couldn't be opened!");
        exit(1);
    }

    int buffer = 0;
    int buffer_counter = 0;
    int bit_count = 0;
    int read;
    while ((read = fgetc(input_handle)) != EOF){
        encode_node* curr = table -> table[search_index(table, read)].encoding;
        while (curr != NULL){
            buffer <<= 1;
            buffer |= curr -> bit;
            buffer_counter = buffer_counter + 1;

            if (buffer_counter == 8) {
                fputc(buffer, output_handle);
                buffer = 0;
                buffer_counter = 0;
            }
            curr = curr->next_bit;
            bit_count = bit_count + 1;
        }
    }
    if (buffer_counter > 0) {
        buffer <<= (8 - buffer_counter);
        fputc(buffer, output_handle);
    }

    fseek(output_handle, 0, SEEK_SET);
    fwrite(&bit_count, sizeof(int), 1, output_handle);

    fclose(input_handle);
    fclose(output_handle);
    printf("Compressing successful!");
}

int main(){
    char* input_file = "mytext.txt";
    char* output_file = "myout.bin";
    FILE* handle = fopen(input_file, "rb");

    if (handle != NULL){
        int char_limit = 256;
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
        encode_file(input_file, output_file, dictionary);        
    }else{
        printf("Error opening file");
        exit(1);
    }
    
    return 0;
}
