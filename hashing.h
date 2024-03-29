#ifndef HASHING_H
#define HASHING_H

typedef struct hash_item
{
    char letter;
    int frequency;
    unsigned char encode[]; // this thing right here is the source of my agony. dyn allocate it and it doesn't work
}hash_item;                 // give it a constant value and the program outright crashes...

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


#endif