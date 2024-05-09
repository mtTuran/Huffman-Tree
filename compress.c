#include <stdio.h>
#include <stdlib.h>
#include "huffmanfun.h"
#include "hashing.h"

int char_limit = 288;

void order_in_canonical_format(queue** head, hash_table* table){
    for (int i = 0; i < char_limit; i = i + 1){
        if (table -> table[i].letter != '\0') insert_to_queue(head, create_leaf(table -> table[i].code_length, table -> table[i].letter));
    }
}

void find_canonical_length(hash_table* table, node* root, int bit_length){
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL){
        int index = search_index(table, root -> letter);
        assign_canonical_length(table, root -> letter, bit_length);
    }

    if (root->left){
        find_canonical_length(table, root -> left, bit_length + 1);
    }

    if (root->right){
        find_canonical_length(table, root -> right, bit_length + 1);
    }
}

void assign_encodings(hash_table* table, queue** ordered_letters) {
    unsigned int code = 0;
    unsigned int* bits = (unsigned int*) malloc(sizeof(unsigned int));
    unsigned int prev_bit_length = 1;
    unsigned int auxilary_counter = 0;
    queue* curr = *ordered_letters;
    while (curr != NULL) {
        node* current_letter_element = curr->element;
        char letter = current_letter_element->letter;
        int index = search_index(table, letter);
        int code_length = table -> table[index].code_length;
        if (prev_bit_length != code_length){
            bits = (unsigned int*) realloc(bits, sizeof(unsigned int) * code_length);
            for (int i = 0; i < (code_length - prev_bit_length); i = i + 1){
                code = code << 1;
            }
            prev_bit_length = code_length;            
        }
        auxilary_counter = 0;
        for (int i = code_length - 1; i >= 0; i = i - 1) {
            bits[auxilary_counter] = (code >> i) & 0x1;
            auxilary_counter = auxilary_counter + 1;
        }
        
        encode_letter(table, letter, bits, code_length);

        code = code + 1;

        curr = curr->next_element;
    }
}

encode_node* serialize_huffman(queue** ordered_letters, int* significant_bit_number) {
    queue* letters = *ordered_letters;
    if (letters == NULL){
        printf("No letters found in order!");
        return NULL;
    }
    encode_node* head = NULL;
    encode_node* current_bit = NULL;
    (*significant_bit_number) = 0;
    while (letters != NULL){
        node* curr_element = pop(&letters);
        char curr_letter = curr_element -> letter;
        int encoding_length = curr_element -> recurrence;
         for (int i = 7; i >= 0; i = i - 1) {
            if (head == NULL) {
                head = create_encode_node(((curr_letter) >> i) & 0x1);
                current_bit = head;
            } else {
                current_bit->next_bit = create_encode_node(((curr_letter) >> i) & 0x1);
                current_bit = current_bit->next_bit;
            }
        }
        for (int i = 31; i >= 0; i = i - 1){
            current_bit->next_bit = create_encode_node(((encoding_length) >> i) & 0x1);
            current_bit = current_bit->next_bit;
        }
        (*significant_bit_number) = (*significant_bit_number) + 8 + 32;
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
    fwrite(&tree_info_bits, sizeof(int), 1, output_handle);
    unsigned char buffer = 0;
    int character_bit_length = 0;

    encode_node* curr = serialized_tree;
    while (curr != NULL) {
        for (int i = 0; i < 8; i = i + 1){
            buffer <<= 1;
            printf("%d ", curr -> bit);
            buffer |= ((curr -> bit >> 0) & 1); // take the least significant bit which is set to either 0 or 1
            curr = curr->next_bit;
        }
        fputc(buffer, output_handle);   // put the character in file
        printf("bit length of %c: ", buffer);
        buffer = 0;
        for (int i = 0; i < 32; i = i + 1){
            character_bit_length <<= 1;
            character_bit_length |= ((curr -> bit >> 0) & 1); // take the least significant bit which is set to either 0 or 1
            curr = curr->next_bit;
        }
        fwrite(&character_bit_length, sizeof(int), 1, output_handle);
        printf("%d\n", character_bit_length);
        character_bit_length = 0;
    }
    // since we do not know the bit number that the text will need, we need placeholder memory for backtracking
    int placeholder_int = 0;
    fwrite(&placeholder_int, sizeof(int), 1, output_handle);    // number of bits that the text will need
    buffer = 0;
    int buffer_counter = 0;
    int file_bit_count = 0;
    int read;
    while ((read = fgetc(input_handle)) != EOF){
        encode_node* curr = table -> table[search_index(table, read)].encoding;
        while (curr != NULL){
            buffer <<= 1;
            buffer |= ((curr -> bit >> 0) & 1);
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

    int tree_byte = (tree_info_bits / 8) + ((tree_info_bits % 8) > 0);  // number of significant bytes the tree needs
    fseek(output_handle, sizeof(int) + (sizeof(unsigned char) * tree_byte), SEEK_SET);
    fwrite(&file_bit_count, sizeof(int), 1, output_handle);
    

    fclose(input_handle);
    fclose(output_handle);
    printf("Compressing successful!");
}

int main(){
    char* input_file = "testText.txt";
    char* output_file = "compressedTest.bin";
    FILE* handle = fopen(input_file, "rb");

    if (handle != NULL){
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
        
        queue* auxilary_queue = NULL;   // to find the canonical encodings
        find_canonical_length(dictionary, father, 0);
        order_in_canonical_format(&auxilary_queue, dictionary);
        assign_encodings(dictionary, &auxilary_queue);

        int tree_info_bits = 0;
        encode_node* serialized_huffman = serialize_huffman(&auxilary_queue, &tree_info_bits);
        encode_file(input_file, output_file, dictionary, serialized_huffman, tree_info_bits);
        
    }else{
        printf("Error opening file");
        exit(1);
    }
    
    return 0;
}
