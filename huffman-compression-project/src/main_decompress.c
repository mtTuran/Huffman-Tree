#include <stdio.h>
#include <stdlib.h>
#include "../include/huffman.h"

int tree_info_bits;
node* create_empty_node(){
    node* new_node = (node*) malloc(sizeof(node));
    new_node -> letter = '\0';
    new_node -> recurrence = 0;
    new_node -> left = NULL;
    new_node -> right = NULL;
    return new_node;
}

node* rebuild_tree(FILE* handle){    
    fread(&tree_info_bits, sizeof(int), 1, handle);
    unsigned char letter = 0;
    int bit_length = 0;
    int number_of_nodes = tree_info_bits / (8 * 5); // 1 byte for character + 4 byte for it's bit length
    queue* reverse_ordered_letters = NULL;
    for (int i = 0; i < number_of_nodes; i = i + 1){
        fread(&letter, sizeof(letter), 1, handle);
        fread(&bit_length, sizeof(bit_length), 1, handle);
        alt_insert(&reverse_ordered_letters, create_leaf(bit_length, letter));
    }

    node* root = create_empty_node();
    unsigned int code = 0;
    unsigned int prev_bit_length = 1;
    while (number_of_nodes > 0) {
        node* current_letter_element = pop(&reverse_ordered_letters);
        char letter = current_letter_element -> letter;
        int code_length = current_letter_element -> recurrence; // bit length in this case
        if (prev_bit_length != code_length){
            for (int i = 0; i < (code_length - prev_bit_length); i = i + 1){
                code = code << 1;
            }
            prev_bit_length = code_length;            
        }
        node* curr = root;
        for (int i = code_length - 1; i >= 0; i = i - 1){
            if(((code >> i) & 1) == 1){
                if(curr -> left) curr = curr -> left;
                else{
                    curr -> left = create_empty_node();
                    curr = curr -> left;
                }
            }
            else{
                if(curr -> right) curr = curr -> right;
                else{
                    curr -> right = create_empty_node();
                    curr = curr -> right;
                }
            }
        }
        curr -> letter = letter;
        curr -> recurrence = code_length;
        curr = root;        
        code = code + 1;
        number_of_nodes -= 1;
    }
    printf("Tree reconstructed successfully.\n");        
    return root;
}

void inorder_traversal(node* root) {
    if (root) {       
        inorder_traversal(root->left);
        if (root -> letter != '\0') printf("%c: %d\n", root -> letter, root -> recurrence);  
        inorder_traversal(root->right);
    }
}

void decode_file(char* input_file, char* output_file, node* root){
    FILE* input_handle = fopen(input_file, "rb");
    FILE* output_handle = fopen(output_file, "wb");

    if (!input_handle || !output_handle){
        printf("File couldn't be opened!");
        exit(1);
    }

    fseek(input_handle, (sizeof(int) + sizeof(unsigned char) * (tree_info_bits / 8)), SEEK_SET);
    int significant_bit_number;
    fread(&significant_bit_number, sizeof(significant_bit_number), 1, input_handle);
    
    int byte_number = (significant_bit_number / 8) + (significant_bit_number % 8 > 0);
    unsigned char read;
    node* curr = root;
    for (int i = 0; i < byte_number; i = i + 1){
        fread(&read, sizeof(read), 1, input_handle);
        for (int j = 7; j >= 0; j = j - 1){
            if(((i * 8) + (7 - j)) < significant_bit_number){
                if ((((read) >> j) & 1) == 1) curr = curr -> left;
                else curr = curr -> right;

                if (curr -> letter != '\0'){
                    fwrite(&(curr -> letter), sizeof(curr -> letter), 1, output_handle);
                    curr = root;
                }
            }
        }
    }

    fclose(input_handle);
    fclose(output_handle);
    printf("Decoding successful!\n");

}

void print_codes_helper(node* root, int code[], int top) {
    if (root->left) {
        code[top] = 1;
        print_codes_helper(root->left, code, top + 1);
    }
    if (root->right) {
        code[top] = 0;
        print_codes_helper(root->right, code, top + 1);
    }
    if (!(root->left) && !(root->right)) {
        printf("%c: ", root->letter);
        for (int i = 0; i < top; i++) {
            printf("%d", code[i]);
        }
        printf("\n");
    }
}

void print_codes(node* root) {
    int code[100];
    int top = 0;
    if (root == NULL) {
        printf("Tree is empty.\n");
        return;
    }
    print_codes_helper(root, code, top);
}

int main(){
    char* input_file = "../data/compressedTest.bin";
    char* output_file = "../data/decodedTextTest.txt";

    FILE* handle = fopen(input_file, "rb");
    if (handle != NULL){
        printf("File opened successfully.\n");
        node* root = rebuild_tree(handle);
        decode_file(input_file, output_file, root);
    }
    else{
        printf("An error occurred while opening the input file!");
    }
    
    return 0;
}