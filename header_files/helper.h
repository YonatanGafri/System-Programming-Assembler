
/**
 * @brief Modifies a filename based on the specified flag.
 *
 * This function modifies a filename by either appending a postfix to the end or replacing the existing postfix
 * with a new one, assuming the new postfix contains a dot.
 *
 * @param filename A pointer to a constant string representing the original filename.
 * @param postfix A pointer to a constant string representing the postfix (must contain a dot) to add or change.
 * @param flags An integer flag indicating whether to ADD or CHANGE the postfix.
 * @param allocation_error A pointer to a flag that indicates if there was an allocation error.
 * @return A dynamically allocated string containing the modified filename. The caller is responsible for freeing the memory allocated for the new filename.
 */
char* add_postfix(const char *filename, const char *postfix, int flags, flag *allocation_error);

/**
 * @brief Reads a word from the file and returns it.
 *
 * This function reads a word from the file and returns it. A word is defined as a sequence of characters
 * terminated by whitespace, a comma, or a newline.
 *
 * @param file The file pointer from which to read the word.
 * @param allocation_error A pointer to a flag that indicates if there was an allocation error.
 * @return A pointer to the dynamically allocated word, or NULL if no word is found. The caller is responsible for freeing the memory allocated for the word.
 */
char* fget_word(FILE *file, flag *allocation_error);

/**
 * @brief Reads a line from the file and returns it.
 *
 * This function reads a line from the file and returns it.
 *
 * @param file The file pointer from which to read the line.
 * @param allocation_error A pointer to a flag that indicates if there was an allocation error.
 * @return A pointer to the dynamically allocated line, or NULL if no line is found. The caller is responsible for freeing the memory allocated for the line.
 */
char* fget_line(FILE *file, flag *allocation_error);

/**
 * @brief Prints an error message with file name and line number information.
 *
 * This function formats and prints an error message, including the file name
 * and line number where the error occurred. If the line number is negative - meaning it's not in a specific line,
 * it is omitted from the output.
 *
 * @param error The error message to print.
 * @param line The line number where the error occurred. If negative, the line number is omitted.
 * @param file_name The name of the file where the error occurred.
 */
void print_error(char *error, int line, const char *file_name);

/**
 * @brief Retrieves the next word from a pointer.
 *
 * This function retrieves the next word from the provided pointer.
 *
 * @param ptr The pointer to the string from which to retrieve the word.
 * @param allocation_error A pointer to a flag that indicates if there was an allocation error.
 * @return A pointer to the dynamically allocated word, or NULL if no word is found. The caller is responsible for freeing the memory allocated for the word.
 */
char *get_word(char *ptr, flag *allocation_error);

/**
 * @brief Handles allocation errors and frees allocated memory.
 *
 * This function handles allocation errors by freeing allocated memory and printing an error message.
 *
 * @param format A format string indicating the types of arguments ('s' for string, 'l' for list, 'm' for macro).
 * @param ... The arguments to be freed.
 */
void allocation_error_handle(char *format, ...);


int find_word_in_file(char *filename, char *word);