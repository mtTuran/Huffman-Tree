#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    unsigned char letter;
    struct node* left;
    struct node* right;
    struct node* prev;
}node;

node* create_node(char letter, node* prev){
    node* new_node = (node*) malloc(sizeof(node));
    new_node -> letter = letter;
    new_node -> left = NULL;
    new_node -> right = NULL;
    new_node -> prev = prev;
    return new_node;
}

node* rebuild_tree(FILE* handle){
    node* root = create_node('\0', NULL);
    node* curr = root;
    int tree_stream;
    fread(&tree_stream, sizeof(int), 1, handle);
    int loop = (tree_stream / 8);   // remainder of the bits are irrelevant since the encoding must end with a letter
    unsigned char byte;             // program must read another byte anyway
    unsigned char letter = 0;
    int expect_letter = 0;
    for (size_t i = 0; i < loop; i = i + 1){
        fread(&byte, sizeof(unsigned char), 1, handle);
        for (size_t j = 0; j < 8; j++){
            int curr_bit = (byte >> j) & 0x01;
            if (expect_letter){
                for (size_t k = 0; k < 8; k = k + 1){
                    if (j == 8){    // need to read another byte to read the rest of the letter's bits after encountering a leaf node
                        j = 0;      // start shifting bits to letter from beginnig of the next byte
                        i = i + 1;  // read another byte to complete the letter
                        fread(&byte, sizeof(unsigned char), 1, handle);
                    }                   
                    letter <<= 1;
                    letter |= (curr_bit & 1);
                    j = j + 1;
                    curr_bit = (byte >> j) & 0x01;
                }
                
                if (!(curr -> left)){
                    curr -> left = create_node(letter, curr);
                }
                else{
                    curr -> right = create_node(letter, curr);
                    curr = curr -> prev;
                }
                expect_letter = 0;
            }
            else if (curr_bit == 0){
                if (!(curr -> left)){
                    curr -> left = create_node('\0', curr);
                    curr = curr -> left;
                }
                else{
                    curr -> right = create_node('\0', curr);
                    curr = curr -> right;
                }
            }
            else if(curr_bit == 1){
                expect_letter = 1;
            }
        }
    }
    return root;
}

void inorder_traversal(node* root) {
    if (root) {
        if (root == NULL)
        {
            return;
        }
        
        inorder_traversal(root->left);
        printf("%c ", root -> letter);  
        inorder_traversal(root->right);
    }
}


int main(){
    char* input_file = "mykeyedlong_text.bin";
    char* output_file = "mydecoded_text.txt";

    FILE* handle = fopen(input_file, "rb");
    if (handle != NULL){
        printf("File opened successfully.\n");
        node* root = rebuild_tree(handle);
        printf("Tree reconstructed successfully.\n");    
        inorder_traversal(root);    

    }
    else{
        printf("An error occurred while opening the input file!");
    }
    
    return 0;
}