#ifndef HASHING_H
#define HASHING_H

typedef struct hash_item
{
    char letter;
    int frequency;
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
int search_table(hash_table* table, char letter);


#endif