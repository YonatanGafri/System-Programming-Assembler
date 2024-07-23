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

/**
 * @brief Returns the type of the label.
 *
 * This function returns the label_type of the specified node.
 *
 * @param node Pointer to the node.
 * @return The label_type of the node.
 */
int return_type(list *node);

void print_list(list *node); /*to erase*/