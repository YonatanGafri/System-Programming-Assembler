
/**
 * @brief Performs the second pass of the assembler.
 *
 * This function processes the encoded lines and updates the addresses of the labels.
 * If a label is not declared or declared as entry, it will print an error message.
 * The function modifies the values of the encoded lines based on the label addresses.
 *
 * @param file_name The name of the file being processed.
 * @param command_list The list of commands (labels) and their attributes.
 * @param encoded_line The list of encoded lines to be updated.
 * @return OFF if no errors were found, otherwise returns ON.
 */
flag second_assembler(char *file_name, list *command_list, list *encoded_line);

/**
 * @brief Prints the output files for the assembler.
 *
 * This function generates the .ob, .ext, and .ent files based on the encoded lines and data image.
 * It prints the encoded lines and data lines to the .ob file, and any external and entry labels to the .ext and .ent files respectively.
 *
 * @param name The base name of the files being processed.
 * @param encoded_line The list of encoded lines.
 * @param data_image The list of data lines.
 * @param command_line The list of commands (labels) and their attributes.
 */
void print_output_files(char *name, list *encoded_line, list *data_image, list *command_line);

/**
 * @brief Counts the number of elements in a linked list.
 *
 * This function traverses the linked list and counts the number of elements.
 *
 * @param head The head of the linked list.
 * @return The number of elements in the linked list.
 */
int count_list(list* head);

/**
 * @brief Returns the type of a symbol in the linked list.
 *
 * This function searches for a symbol in the linked list and returns its type.
 *
 * @param name The name of the symbol to search for.
 * @param linked_list The linked list to search in.
 * @return The type of the symbol if found, otherwise -1.
 */
int return_type_symbol(char* name, list *linked_list);