/*functions in pre_assembler*/

typedef struct Macro Macro;

/**
 * @brief Preprocesses an assembler file by expanding macros defined within it.
 *
 * This function reads an input assembler file, identifies macro definitions, 
 * and replaces instances of those macros with their expanded definitions in a new output file.
 *
 * @param filename The name of the input assembler file.
 * @param command_list Pointer to the list of commands (modified by the function).
 * @return OFF if no errors were found, otherwise returns ON.
 */
flag pre_assembler(const char* filename, list *command_list);

/**
 * @brief Creates a new macro structure and initializes it with the given name.
 *
 * This function allocates memory for a new macro structure, initializes it with the given name,
 * and returns a pointer to the new macro.
 *
 * @param name The name of the macro to create.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return Pointer to the newly created macro, or NULL if an allocation error occurred.
 */
Macro* create_macro(const char*, flag *allocation_error);


/**
 * @brief Adds a line to the specified macro structure.
 *
 * This function adds a line to the specified macro structure, reallocating memory if necessary
 * to accommodate additional lines.
 *
 * @param macro Pointer to the macro structure.
 * @param line The line to add to the macro.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void add_line_to_macro(Macro *macro, const char *line, flag *allocation_error);

/**
 * @brief Frees the memory allocated for the specified macro structure.
 *
 * This function frees the memory allocated for the specified macro structure and all its associated lines.
 *
 * @param current Pointer to the macro structure to free.
 */
void free_macro(Macro*);

/**
 * @brief Performs pre-processing checks on the input file.
 *
 * This function performs pre-processing checks on the input file to identify macro definitions,
 * ensure correct syntax, and detect errors.
 *
 * @param file Pointer to the input file to be processed.
 * @param file_name The name of the file being processed.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return OFF if no errors were found during the check, otherwise returns ON.
 */
flag pre_assembler_check(FILE *file, const char *file_name, flag *allocation_error);

