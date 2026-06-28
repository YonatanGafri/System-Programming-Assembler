/**
 * @file helper.c
 * @brief Contains helper functions for the assembler project, including finding registers,
 * checking labels, finding commands, and validating addressing modes.
 */

#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "assembler.h"


/**
 * @brief Finds the index of a register in the register table.
 *
 * This function searches for the specified register in the register table.
 * If the register is found, its index is returned; otherwise, -1 is returned.
 *
 * @param word The register to search for.
 * @return The index of the register if found, otherwise -1.
 */
int find_register(char *word){
    char *reg_table[] = REG_TABLE;
    int i;

    for(i = 0; i < (sizeof(reg_table)/sizeof(reg_table[0])); i++){
        if(strcmp(reg_table[i], word) == 0)
            return i;
    }
    return -1;
}

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
int check_label(char *word){
    /*!!!assuming a label is good also with a '_' character */
    int i, size;
    if((size = strlen(word)) > MAX_LABEL_LENGTH) /* Check if the length of the word exceeds the maximum allowed label length */
        return -1;
    for(i = 0; i < size; i++){
        if((i == 0 && !isalpha(word[i])) || (!isalnum(word[i]) && word[i] != '_'))  /* Validate each character in the word */
            return -1;
    }
    return 0;
}

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
int find_command(char *word){
    char *command_table[] = COMMAND_TABLE;
    int i;
    for(i = 0; i < LAST_COMMAND; i++) {
        if(strcmp(word, command_table[i]) == 0) {
            return i;  /* Return the index of the command found */
        }
    }
    return -1;  /* Return -1 if the command is not found */
}

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
flag command_options(int command_vector[]){
    int i;
    const int command_table[][3] = COMMAND_OPTIONS_TABLE;

    for(i = 0; i < sizeof(command_table) / sizeof(command_table[0]); i++){
        if(command_table[i][0] == command_vector[0] && command_table[i][1] == command_vector[1] && command_table[i][2] == command_vector[2])
            return OFF;
    }
    return ON;
}

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
int find_addresing(char *word){
    int i;  
    if(word[0] == '#'){
        for(i = 1; i < strlen(word); i++){
            if(!(isdigit(word[i]) || (i == 1 && word[i] == '-'))){
                return -1;
            }
        }
        if(atoi(word+1) > MAX_NUMBER_12BIT || atoi(word+1) < MIN_NUMBER_12BIT) 
            return -1;
        return IMMEDIATE_ADDRESING;
    }
    else if(word[0] == '*'){
        if(find_register(word+1) == -1)
            return -1;
    return INDIRECT_REGISTER_ADDRESING;
    }
    else if(find_register(word) >= 0)
        return DIRECT_REGISTER_ADDRESING;
    else return DIRECT_ADDRESING;
}

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
flag check_lable_define(char *line, list *command_list, int line_num, char *file_name, flag *allocation_error){
    char *temp = line, *word = NULL;
    int i;
    list *temp_node;

    for(i = 0; i < strlen(temp) && (temp[i] == ' ' || temp[i] == '\t' || temp[i] == ','); i++){
            if(temp[i] == ','){
                print_error("Illegal comma", line_num, file_name);
                return ON;
            }
    }
    temp += i;
    word = get_word(temp, allocation_error);
    if(word[strlen(word) -1] == ':'){   /*the first word is a lable*/
        word[strlen(word)-1] = '\0'; 
        temp += (strlen(word)+1);
        if(check_label(word) == -1 || find_command(word) != -1 || ((temp_node = list_search(command_list, word)) != NULL && return_type(temp_node) != ENTRY)){
            print_error("Invalid_lable", line_num, file_name);
            free(word);
            return ON;
        }
        for(i = 0; i < strlen(temp) && (temp[i] == ' ' || temp[i] == '\t' || temp[i] == ','); i++){
            if(temp[i] == ','){
                print_error("Illegal comma", line_num, file_name);
                free(word);
                return ON;
            }
        }
    }
    free(word);
    /*if i want, add a warning of a label before extern and entry*/
    return OFF;
}

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
flag check_line_length(char *line, int line_num, char *file_name){
    if(strlen(line) > MAX_LINE_LENGTH -1){
        print_error("To many characters", line_num, file_name);
        return ON;
    }
    return OFF;
}