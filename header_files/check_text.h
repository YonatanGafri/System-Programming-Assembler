/**
 * @brief Classifies a line as either instructional or a command.
 *
 * This function reads a line and determines if it is an instructional line
 * (starts with a dot) or a command line.
 *
 * @param line The line to classify.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return INSTRUCTIONAL if the line is an instructional line, otherwise COMMAND.
 */
int classify_line(char *line, flag *allocation_error);

/**
 * @brief Checks if an instructional line is valid.
 *
 * This function checks if an instructional line contains valid instructions and arguments.
 *
 * @param command_list Pointer to the list of commands.
 * @param line The line to check.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return ON if an error is found, otherwise OFF.
 */
flag check_instructional_line(list *command_list, char *line, int line_num, char *file_name, flag *allocation_error);

/**
 * @brief Checks if a data instruction line is valid.
 *
 * This function checks if a data instruction line contains valid data values.
 *
 * @param line The line to check.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return ON if an error is found, otherwise OFF.
 */
flag check_data_instruction(char *line, int line_num, char *file_name, flag *allocation_error);

/**
 * @brief Checks if a string instruction line is valid.
 *
 * This function checks if a string instruction line contains a valid string.
 *
 * @param line The line to check.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @return ON if an error is found, otherwise OFF.
 */
flag check_string_instruction(char *line, int line_num, char *file_name);

/**
 * @brief Checks if an entry instruction line is valid.
 *
 * This function checks if an entry instruction line contains valid entry labels.
 *
 * @param command_list Pointer to the list of commands.
 * @param line The line to check.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return ON if an error is found, otherwise OFF.
 */
flag check_entry_instruction(list *command_list, char *line, int line_num, char *file_name, flag *allocation_error);

/**
 * @brief Checks if an extern instruction line is valid.
 *
 * This function checks if an extern instruction line contains valid extern labels.
 *
 * @param command_list Pointer to the list of commands.
 * @param line The line to check.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return ON if an error is found, otherwise OFF.
 */
flag check_extern_instruction(list *command_list, char *line, int line_num, char *file_name, flag *allocation_error);


/**
 * @brief Checks if a command line is valid.
 *
 * This function checks if a command line contains valid commands and operands.
 *
 * @param line The line to check.
 * @param command_list Pointer to the list of commands.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return ON if an error is found, otherwise OFF.
 */
flag check_command_line(char *line, list *command_list, int line_num, char *file_name, flag *allocation_error);