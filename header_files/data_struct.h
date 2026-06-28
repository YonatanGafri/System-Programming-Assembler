#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Opaque pointer to hide the structure */
typedef struct list list;

/**
 * @brief Creates a new list element.
 * 
 * This function allocates memory for a new list element and initializes it.
 *
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return Pointer to the newly created list element, or NULL if allocation fails.
 */
list* create_new_list(flag *allocation_error);

/**
 * @brief Adds an element to the command list.
 * 
 * This function creates a new element and adds it to the provided linked list.
 *
 * @param command_list Pointer to the list of commands.
 * @param name The name of the command/macro.
 * @param value The value associated with the command.
 * @param label_type The type of the label (MACRO, UNINITIALIZED, etc.).
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void add_to_list(list *command_list, char *name, int value, int label_type, flag *allocation_error);

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
list *list_search(list *command_list, char *name);

/**
 * @brief Frees the command list.
 *
 * This function frees all elements in the provided linked list.
 *
 * @param command_list Pointer to the list of commands.
 */
void free_list(list *command_list);

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
void edit_or_add_to_list(list *command_list, char *name, int value, int label_type, flag *allocation_error);


int return_type(list *node);

char* return_name(list *noed);

int return_value(list *node);

list* return_next(list *node);

void change_value(list *node, int new_val);

void change_type(list *node, int new_type);

/**
 * @brief Creates a new empty list element.
 *
 * This function allocates memory for a new list element, initializes its fields
 * to default values (NULL or UNINITIALIZED), and returns a pointer to it.
 *
 * @param allocation_error Pointer to a flag that is set to ON if memory allocation fails.
 * @return Pointer to the newly created list element, or NULL if memory allocation fails.
 */
list *sort_list(list *node, flag *allocation_error);