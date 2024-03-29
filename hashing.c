#include <stdlib.h>
#include <stdio.h>
#include "hashing.h"

hash_table* create_hash_table(int size){
    hash_table* new_map = (hash_table*) malloc(sizeof(hash_table) + sizeof(hash_item) * size + sizeof(unsigned char) * size);
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

int search_index(hash_table* table, char letter){
    int collision = 0;
    int success = 0;
    int size = table -> size;
    int index = hash_function(letter, collision, size);


    while (!success){
        if (index >= 0 && index < size && collision < size){
            if(table -> table[index].letter == letter){
                success = 1;
                return index;
            }
            else if(table -> table[index].letter == '\0'){
                printf("An empty index found in letter searching process.\nSize of the map: %d\nCalculated index: %d\nCollision: %d", size, index, collision);
                exit(1);
            }
            else{
                collision += 1;
                index = hash_function(letter, collision, size);
            }
        }
        else{
            printf("An error occurred in the index searching process.\nSize of the map: %d\nCalculated index: %d\nCollision: %d", size, index, collision);
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

char* create_search_list(int size){
    char* search_list = (char*) malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++) {
        search_list[i] = '\0';
    }
    return search_list;
}

void add_to_search_list(char search_list[], int size, char letter){
    int flag = 0;
    for(int i = 0; i < size; i = i + 1){
        if (flag){
            break;
        }
        else if(search_list[i] == letter){
            flag = 1;
        }
        else if(search_list[i] == '\0'){
            search_list[i] = letter;
            break;
        }
    }
}