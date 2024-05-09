#include <stdio.h>
#include <stdlib.h>
#include "huffmanfun.h"

int tree_info_bits;
node* rebuild_tree(FILE* handle){    
    fread(&tree_info_bits, sizeof(int), 1, handle);
    unsigned char letter = 0;
    int bit_length = 0;
    int number_of_nodes = tree_info_bits / (8 * 5); // 1 byte for character + 4 byte for it's bit length
    queue* reverse_ordered_letters = NULL;
    for (int i = 0; i < number_of_nodes; i = i + 1){
        fread(&letter, sizeof(letter), 1, handle);
        fread(&bit_length, sizeof(bit_length), 1, handle);
        reverse_insert(&reverse_ordered_letters, create_leaf(bit_length, letter));
    }

    node* root = NULL;
    while (number_of_nodes > 1){
        node* small = pop(&reverse_ordered_letters);
        node* big = pop(&reverse_ordered_letters);
        root = huffman(small, big);
        reverse_insert(&reverse_ordered_letters, root);
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
    char* input_file = "compressedTest.bin";
    char* output_file = "decodedTextTest.txt";

    FILE* handle = fopen(input_file, "rb");
    if (handle != NULL){
        printf("File opened successfully.\n");
        node* root = rebuild_tree(handle);
        //print_codes(root);
        //decode_file(input_file, output_file, root);
        inorder_traversal(root);
    }
    else{
        printf("An error occurred while opening the input file!");
    }
    
    return 0;
}