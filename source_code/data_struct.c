/**
 * @file data_struct.c
 * @brief Implementation of a linked list data structure for managing commands and macros in an assembler project.
 *
 * This file contains functions to create, add to, search, and free a linked list of commands and macros.
 */

#include "definitions.h"
#include "data_struct.h"


struct list {
    char *name;
    int value;
    int label_type;
    struct list *next;
};

/**
 * @brief Creates a new list element.
 * 
 * This function allocates memory for a new list element and initializes it.
 *
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return Pointer to the newly created list element, or NULL if allocation fails.
 */
list* create_new_list(flag *allocation_error){
    list *new_list = (list*)malloc(sizeof(list));
    if(new_list == NULL){
        *allocation_error = ON;
        return NULL;
    }
    new_list->name = NULL;
    new_list->value = UNINITIALIZED;
    new_list->label_type = UNINITIALIZED;
    new_list->next = NULL;

    return new_list;
}

/**
 * @brief Adds an element to the command list.
 * 
 * This function creates a new element and adds it to the end of the provided linked list.
 *
 * @param command_list Pointer to the list of commands.
 * @param name The name of the command/macro.
 * @param value The value associated with the command.
 * @param label_type The type of the label (MACRO, UNINITIALIZED, etc.).
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void add_to_list(list *command_list, char *name, int value, int label_type, flag *allocation_error) {
    list *current = command_list, *temp;  

    if(command_list->name == NULL && command_list->value == UNINITIALIZED && command_list->label_type == UNINITIALIZED && command_list->next == NULL){
        if(name != NULL){
            command_list->name = (char*)malloc((strlen(name)+1)*sizeof(char));
            if (command_list->name == NULL) {
                *allocation_error = ON;
                return;
            }
            /* Initialize the new element */
            strcpy(command_list->name, name);
        }
        command_list->value = value;
        command_list->label_type = label_type;
        return;
    }
    temp = (list*)malloc(sizeof(list));
    if(temp == NULL){
        *allocation_error = ON;
        return;
    }

    if(name != NULL){
        temp->name = (char*)malloc((strlen(name)+1)*sizeof(char));
        if (temp->name == NULL) {
            *allocation_error = ON;
            return;
        }
        /* Initialize the new element */
        strcpy(temp->name, name);
    }
    else temp->name = NULL;
    

    temp->value = value;
    temp->label_type = label_type;
    temp->next = NULL;
    
    while(current->next != NULL) current = current->next; /*move to the last node in the list*/

    current->next = temp;  /* Point the new element to the head's next element */ 
}

/**
 * @brief Searches for an element in the command list.
 *
 * This function searches for an element with the specified name in the provided
 * linked list. If the element is found, a pointer to it is returned.
 * If the element is not found, NULL is returned.
 *
 * @param command_list Pointer to the linked list.
 * @param name The name of the element to search for.
 * 
 * @return Pointer to the found element, or NULL if not found.
 */
list *list_search(list *command_list, char *name){
    list *current = command_list;
    if(command_list->name == NULL && command_list->value == UNINITIALIZED && command_list->label_type == UNINITIALIZED && command_list->next == NULL){ /* Check if the list is empty */
        return NULL;
    }
    do{        /* Traverse the linked list */
        if (current->name != NULL && strcmp(current->name, name) == 0){
            return current;  /* Return the found element */
        }
        current = current->next;
    }while(current != NULL);
    return NULL;  /* Return NULL if the element is not found */
}

/**
 * @brief Frees the command list.
 *
 * This function frees all elements in the provided linked list.
 *
 * @param command_list Pointer to the list of commands.
 */
void free_list(list *command_list){
    list *temp, *current = command_list;
    
    if (command_list == NULL) return;
    do{
        temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    } while (current != NULL);  /* Loop until we come back to the starting point */
}

/**
 * @brief Edits an existing element or adds a new element to the command list.
 *
 * This function searches for an element with the specified name in the provided
 * linked list. If the element is found, it updates its value and label_type.
 * If the element is not found, it adds a new element to the list.
 *
 * @param command_list Pointer to the list of commands.
 * @param name The name of the command/macro.
 * @param value The value associated with the command.
 * @param label_type The type of the label (MACRO, UNINITIALIZED, etc.).
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void edit_or_add_to_list(list *command_list, char *name, int value, int label_type, flag *allocation_error){
    list *temp = list_search(command_list, name);
    int i;
    if(temp == NULL){
        add_to_list(command_list, name, value, label_type, allocation_error);
        if(*allocation_error == ON) return;
    }
    else{
        free(temp->name);
        temp->name = (char*)malloc(sizeof(char)*strlen(name)+1);
        if(temp->name == NULL){
            *allocation_error = ON;
            return;
        }
        for(i = 0; i <= strlen(name); i++) temp->name[i] = name[i];
        temp->value = value;
        temp->label_type = label_type;
    }
}

/**
 * @brief Returns the type of the label.
 *
 * This function returns the label_type of the specified node.
 *
 * @param node Pointer to the node.
 * @return The label_type of the node.
 */
int return_type(list *node){
    return node->label_type;
}

char* return_name(list *node){
    return node->name;
}

int return_value(list *node){
    return node->value;
}

list* return_next(list *node){
    return node->next;
}

void change_value(list *node, int new_val){
    node->value = new_val;
}

void change_type(list *node, int new_type){
    node->label_type = new_type;
}


list *sort_list(list *node, flag *allocation_error){
    list *sorted = NULL; /* Create a new sorted (empty) list */
    list *current = node;
    list *next;
    list *new_node, *temp, *prev;

    

    if(node->name == NULL && node->label_type == UNINITIALIZED && node->value == UNINITIALIZED)
        return NULL;

    while (current != NULL) {
        next = current->next;

        new_node = (list *)malloc(sizeof(list));
        if (new_node == NULL) {
            *allocation_error = ON;
            return NULL;
        }
        new_node->name = (char *)malloc(strlen(current->name) + 1);
        if (new_node->name == NULL){
            free(new_node);
            *allocation_error = ON;
            return NULL;
        }
        strcpy(new_node->name, current->name);
        new_node->value = current->value;
        new_node->label_type = current->label_type;
        new_node->next = NULL;

        if(sorted == NULL || sorted->value > new_node->value){ /*didnt start sorting or the value is smaller the the first element*/
            new_node->next = sorted; /*this is the first*/
            sorted = new_node;
        }else{
            temp = sorted;
            prev = NULL;

            while(temp != NULL && temp->value < new_node->value){ /*enter the node at the right place*/
                prev = temp;
                temp = temp->next;
            }

            if (prev == NULL) { 
                new_node->next = sorted;
                sorted = new_node;
            } else {
                new_node->next = temp;
                prev->next = new_node;
            }
        }

        current = next;
    }

    return sorted;
}