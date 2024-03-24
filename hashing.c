#include <stdlib.h>
#include <stdio.h>
#include "hashing.h"

hash_table* create_hash_table(int size){
    hash_table* new_map = (hash_table*) malloc(sizeof(hash_table) + sizeof(hash_item) * size);
    new_map -> size = size;
    new_map -> count = 0;

    for (int i = 0; i < size; i++) {
    new_map->table[i].letter = '\0';
    new_map->table[i].frequency = 0;
    }

    return new_map;
}

int hash_function(char letter, int collision, int size){
    int index = ((int)letter + collision) % size;
    return index;
}

void map_to_table(hash_table* table, char letter){
    int collision = 0;
    int success = 0;
    int size = table -> size;
    int index = hash_function(letter, collision, size);

    while (!success){ 
        if (index >= 0 && index < size && collision < size) {
            if (table -> table[index].letter == '\0') {
                table -> table[index].letter = letter;
                table -> table[index].frequency += 1;
                table -> count = table -> count + 1;
                success = 1;
            }
            else if (table -> table[index].letter == letter){
                table -> table[index].frequency += 1;
                success = 1;
            }
            else {
                collision += 1;
                index = hash_function(letter, collision, size);
            }
        } else {
            printf("An error occurred in the mapping process.\nSize of the map: %d\nCalculated index: %d\nCollision: %d", size, index, collision);
            exit(1);
        }
    }
}


int search_table(hash_table* table, char letter){
    int collision = 0;
    int success = 0;
    int size = table -> size;
    int index = hash_function(letter, collision, size);


    while (!success){
        if (index >= 0 && index < size && collision < size){
            if(table -> table[index].letter == letter || table -> table[index].letter == '\0'){
                success = 1;
                return table -> table[index].frequency;
            }
            else{
                collision += 1;
                index = hash_function(letter, collision, size);
            }
        }
        else{
            printf("An error occurred in the searching process.\nSize of the map: %d\nCalculated index: %d\nCollision: %d", size, index, collision);
            exit(1);
        }
    }
}