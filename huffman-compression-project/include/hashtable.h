#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct encode_node
{
    unsigned char bit;
    struct encode_node* next_bit;
}encode_node;


typedef struct hash_item
{
    char letter;
    int frequency;
    int code_length;
    encode_node* encoding;
}hash_item;

typedef struct hash_table
{
    int size;
    int count;
    hash_item table[];
}hash_table;

hash_table* create_hash_table(int size);
int hash_function(char letter, int collision, int size);
void map_to_table(hash_table* table, char letter);
int search_index(hash_table* table, char letter);
int search_table(hash_table* table, char letter);

char* create_search_list(int size);
void add_to_search_list(char search_list[], int size, char letter);

void assign_canonical_length(hash_table* table, char letter, int bit_length);
encode_node* create_encode_node(char bit);
void encode_letter(hash_table* table, char letter, unsigned int* bits, unsigned int bit_length);
int is_power_of_two(unsigned int n);
void print_encode_table(hash_table* table);

#endif