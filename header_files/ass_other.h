/**
 * @brief Finds the index of a register in the register table.
 *
 * This function searches for the specified register in the register table.
 * If the register is found, its index is returned; otherwise, -1 is returned.
 *
 * @param word The register to search for.
 * @return The index of the register if found, otherwise -1.
 */
int find_register(char *word);

/**
 * @brief Checks if a given word is a valid label.
 *
 * A valid label must:
 * - Not exceed the maximum label length.
 * - Start with an alphabet character.
 * - Contain only alphanumeric characters or underscores.(assumed underscure is also good)
 *
 * @param word The word to check for validity as a label.
 * @return 0 if the label is valid, -1 otherwise.
 */
int check_label(char *word);

/**
 * @brief Finds a command in the command table.
 *
 * This function searches for a command in the system command table.
 * If the command is found, its index in the table is returned.
 * If the command is not found, -1 is returned.
 *
 * @param word The command to search for.
 * @return The index of the command in the table if found, otherwise -1.
 */
int find_command(char *word);

/**
 * @brief Checks if a given command vector is valid.
 *
 * This function validates the given command vector against the command options table.
 * If the command vector is found in the table, OFF is returned. Otherwise, ON is returned.
 * A vactor has - the command, first addressing and the second addressing.
 *
 * @param command_vector The command vector to check.
 * @return OFF if the command vector is valid, otherwise ON.
 */
flag command_options(int command_vector[]);

/**
 * @brief Finds the addressing mode of a given operand.
 *
 * This function determines the addressing mode of the specified operand.
 * If the operand is valid, the corresponding addressing mode is returned.
 * If the operand is invalid, -1 is returned.
 *
 * @param word The operand to check.
 * @return The addressing mode of the operand, or -1 if invalid.
 */
int find_addresing(char *word);

/**
 * @brief Checks the definition of a label in a line.
 *
 * This function checks if the first word in a line is a valid label definition.
 * If the label is valid, OFF is returned. Otherwise, an error is printed, and ON is returned.
 *
 * @param line The line to check.
 * @param command_list Pointer to the list of commands.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return OFF if the label definition is valid, otherwise ON.
 */
flag check_lable_define(char *line, list *command_list, int line_num, char *file_name, flag *allocation_error);

/**
 * @brief Checks the length of a line.
 *
 * This function checks if the length of a line exceeds the maximum allowed length.
 * If the line is too long, an error is printed, and ON is returned.
 *
 * @param line The line to check.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @return OFF if the line length is valid, otherwise ON.
 */
flag check_line_length(char *line, int line_num, char *file_name);