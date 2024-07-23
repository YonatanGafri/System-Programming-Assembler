/**
 * @brief Performs the first pass of the assembler.
 *
 * This function reads the input assembler file, processes each line, and generates an intermediate representation
 * of the code and data sections. It also generates a new file with the processed code.
 *
 * @param name The name of the input assembler file.
 * @param command_list Pointer to the list of commands (modified by the function).
 * @return OFF if no errors were found, otherwise returns ON.
 */
flag first_assembler(char *name, list *command_list);

/**
 * @brief Extracts a number from a line of text.
 *
 * This function extracts the next integer from the line of text and returns it.
 * It also sets a flag indicating whether a number was successfully extracted.
 *
 * @param line The line of text from which to extract the number.
 * @param is_num_set Pointer to a flag that indicates whether a number was successfully extracted.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 * @return The extracted integer.
 */
int get_number(char *line, flag *is_num_set, flag *allocation_error);

/**
 * @brief Converts data values from a line of text to binary and stores them in the data image.
 *
 * This function reads a line of text containing data values, converts them to binary,
 * and stores them in the data image at the specified DC address.
 *
 * @param line The line of text containing the data values.
 * @param DC Pointer to the data counter.
 * @param data_image Pointer to the list representing the data image.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void get_data(char *line, int *DC, list *data_image, flag *allocation_error);

/**
 * @brief Converts a string from a line of text to binary and stores it in the data image.
 *
 * This function reads a line of text containing a string, converts each character to binary,
 * and stores them in the data image at the specified DC address. A null terminator is added at the end.
 *
 * @param line The line of text containing the string.
 * @param DC Pointer to the data counter.
 * @param data_image Pointer to the list representing the data image.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void get_string(char *line, int *DC, list *data_image, flag *allocation_error);

/**
 * @brief Processes labels in a line of text and adds them to the data image.
 *
 * This function reads a line of text containing labels, processes each label,
 * and adds them to the data image at the specified DC address.
 *
 * @param line The line of text containing the labels.
 * @param data_image Pointer to the list representing the data image.
 * @param DC Pointer to the data counter.
 * @param label_type The type of the label (e.g., EXTERN, ENTRY).
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void get_labels(char *line ,list *data_image, int *DC , int label_type, flag *allocation_error);

/**
 * @brief Processes a directive line and updates the data image and command list accordingly.
 *
 * This function processes a directive line, identifies the type of directive (e.g., .data, .string, .extern, .entry),
 * and updates the data image and command list accordingly.
 *
 * @param line The directive line to process.
 * @param DC Pointer to the data counter.
 * @param command_list Pointer to the list of commands.
 * @param data_image Pointer to the list representing the data image.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void directive_line(char *line, int *DC, list *command_list, list *data_image, flag *allocation_error);

/**
 * @brief Processes a command line and updates the encoded line list.
 *
 * This function processes a command line, extracts the opcode and operands,
 * and updates the encoded line list with the corresponding binary representation.
 *
 * @param line The command line to process.
 * @param DI Pointer to the instruction counter.
 * @param command_list Pointer to the list of commands.
 * @param encoded_line Pointer to the list representing the encoded lines.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void command_line(char *line, int  *DI, list *command_list, list *encoded_line, flag *allocation_error);

void second_assembler(char *name, list *command_list);