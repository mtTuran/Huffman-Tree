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

encode_node* serialize_huffman(node* root, int* bit_count) {
    if (root == NULL)
        return NULL;

    encode_node* head = NULL;
    encode_node* current = NULL;

    if (root->left != NULL || root->right != NULL) {
        current = (encode_node*)malloc(sizeof(encode_node));
        current->bit = '0';
        current->next_bit = NULL;

        head = current;
        (*bit_count)++;

        encode_node* left_encoded = serialize_huffman(root->left, bit_count);
        encode_node* right_encoded = serialize_huffman(root->right, bit_count);

        if (left_encoded != NULL) {
            current->next_bit = left_encoded;
            while (current->next_bit != NULL) {
                current = current->next_bit;
                (*bit_count)++;
            }
        }

        if (right_encoded != NULL) {
            current->next_bit = right_encoded;
            while (current->next_bit != NULL) {
                current = current->next_bit;
                (*bit_count)++;
            }
        }
    } else {
        current = (encode_node*)malloc(sizeof(encode_node));
        current->bit = '1';
        current->next_bit = (encode_node*)malloc(sizeof(encode_node));
        current->next_bit->bit = root->letter;
        current->next_bit->next_bit = NULL;

        head = current;
        *bit_count += 9;
    }

    return head;
}

void encode_file(char* input_file, char* output_file, hash_table* table, encode_node* serialized_tree, int tree_info_bits){
    FILE* input_handle = fopen(input_file, "rb");
    FILE* output_handle = fopen(output_file, "wb");

    if (!input_handle || !output_handle){
        printf("File couldn't be opened!");
        exit(1);
    }

    int placeholder_int = 0;
    unsigned char placeholder_byte = 0;
    int tree_byte = (tree_info_bits / 8) + ((tree_info_bits % 8) > 0);
    fwrite(&placeholder_int, sizeof(int), 1, output_handle);
    fwrite(&placeholder_byte, sizeof(unsigned char), tree_byte, output_handle);
    fwrite(&placeholder_int, sizeof(int), 1, output_handle);

    unsigned char buffer = 0;
    int buffer_counter = 0;
    int file_bit_count = 0;
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
            file_bit_count = file_bit_count + 1;
        }
    }
    if (buffer_counter > 0) {
        buffer <<= (8 - buffer_counter);
        fputc(buffer, output_handle);
    }

    fseek(output_handle, sizeof(int) + sizeof(unsigned char) * tree_byte, SEEK_SET);
    fwrite(&file_bit_count, sizeof(int), 1, output_handle);

    fseek(output_handle, 0, SEEK_SET);
    fwrite(&tree_info_bits, sizeof(int), 1, output_handle);
    fseek(output_handle, sizeof(int), SEEK_SET);
    encode_node* curr = serialized_tree;
    buffer = 0;
    buffer_counter = 0;
    while (curr != NULL) {
        buffer <<= 1;
        buffer |= curr -> bit;
        buffer_counter = buffer_counter + 1;
        if (curr->bit == '1') {
            curr = curr -> next_bit;
            for (size_t i = 0; i < 8; i = i + 1){
                if (buffer_counter == 8){
                    fputc(buffer, output_handle);
                    buffer = 0;
                    buffer_counter = 0;
                }
                buffer <<= 1;
                buffer |= ((curr -> bit >> i) & 1);
                buffer_counter = buffer_counter + 1;
            }           
        }
        if (buffer_counter == 8) {
            fputc(buffer, output_handle);
            buffer = 0;
            buffer_counter = 0;
        }
        curr = curr->next_bit;        
    }
    if (buffer_counter > 0) {
        buffer <<= (8 - buffer_counter);
        fputc(buffer, output_handle);
    }

    fclose(input_handle);
    fclose(output_handle);
    printf("Compressing successful!");
}

int main(){
    char* input_file = "long_input_text.txt";
    char* output_file = "mykeyedlong_text.bin";
    FILE* handle = fopen(input_file, "rb");

    if (handle != NULL){
        int char_limit = 288;
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

        int tree_info_bits = 0;
        encode_node* serialized_huffman = serialize_huffman(father, &tree_info_bits);
        encode_file(input_file, output_file, dictionary, serialized_huffman, tree_info_bits);        
    }else{
        printf("Error opening file");
        exit(1);
    }
    
    return 0;
}
