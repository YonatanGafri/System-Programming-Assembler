#include "definitions.h"
#include "data_struct.h"
#include "ass_other.h"
#include "helper.h"
#include "check_text.h"

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
int classify_line(char *line, flag *allocation_error){ 
    char *temp = line, *word = NULL, c;
    for(; isspace(*temp); temp++);
    word = get_word(temp, allocation_error);
    if(*allocation_error == ON || word == NULL) return -1;
    c = word[0];
    free(word);
    
    if(c == '.') return INSTRUCTIONAL;
        else return COMMAND;
}

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
flag check_instructional_line(list *command_list, char *line, int line_num, char *file_name, flag *allocation_error){
    char *temp = line, *word = NULL;
    int i;
    flag error = OFF;

    for(i = 0; i < strlen(temp) && (temp[i] == ' ' || temp[i] == '\t' || temp[i] == ','); i++){
        if(temp[i] == ','){
            print_error("Illegal comma", line_num, file_name);
            return ON;
        }
    } temp += i;

    word = get_word(temp, allocation_error);
    if(*allocation_error == ON) return ON;

    /*its not data or string or entry or extern*/
    if(strcmp(word+1, "entry") != 0 && strcmp(word+1, "extern") != 0 && strcmp(word+1, "data") != 0 && strcmp(word+1, "string") != 0){
            print_error("Unknown instruction", line_num, file_name);
            free(word);
            return ON;
    }

    else if(strcmp(word+1, "data") == 0){ /*the line is a data line*/
        if(check_data_instruction(temp+strlen(word), line_num, file_name, allocation_error) == ON){
            free(word);
            return ON;
        }
        if(*allocation_error == ON){
            free(word);
            return ON;
        }  
    }   
          
    else if(strcmp(word+1, "string") == 0){ /*the line is a string line.*/
        if(check_string_instruction(temp+strlen(word), line_num, file_name) == ON){
            free(word);
            return ON;
        }
    }
    else if(strcmp(word+1, "entry") == 0){ /*the line is an entry line !!! in entry an dextern assuming it may have a few operands*/
        if(check_entry_instruction(command_list, temp+strlen(word), line_num, file_name, allocation_error) == ON){
            free(word);
            return ON;
        }
        if(*allocation_error == ON){
            free(word);
            return ON;
        } 
    }
    
    else{ /*the line is an extern line*/
        if(check_extern_instruction(command_list, temp+strlen(word), line_num, file_name, allocation_error) == ON){
            free(word);
            return ON;
        }
        if(*allocation_error == ON){
            free(word);
            return ON;
        } 
    }
    

    free(word);
    return error;
}

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
flag check_data_instruction(char *line, int line_num, char *file_name, flag *allocation_error){
    char *temp = line, *word = NULL;
    int i, num;
    flag is_comma = OFF;
    for(i = 0; i < strlen(temp) && (temp[i] == ' ' || temp[i] == '\t' || temp[i] == ','); i++){
            if(temp[i] == ','){
                print_error("Illegal comma", line_num, file_name);
                return ON;
            }
    }
    temp += i;

    word = get_word(temp, allocation_error);
    if(*allocation_error == ON) return ON;
    while(word != NULL){
        if(!((num = atoi(word)) != 0 || word[0] == '0' || (strlen(word) > 0 && (word[0] == '+' || word[0] == '-') && word[1] == '0'))){
            print_error("Illegal data - not a number", line_num, file_name);
            free(word);
            return ON;
        }
        if(num > MAX_NUMBER || num < MIN_NUMBER){
            print_error("Illegal data - number out of range", line_num, file_name);
            free(word);
            return ON;
        }
        if((double)num != atof(word)){
            print_error("Illegal data - not an intager", line_num, file_name);
            free(word);
            return ON;
        }
        temp += strlen(word);
        is_comma = OFF;
        for(i = 0; i < strlen(temp) && (temp[i] == ' ' || temp[i] == '\t' || temp[i] == ','); i++){
            if(temp[i] == ','){
                if(is_comma == OFF) is_comma = ON;
                else{
                    print_error("Illegal comma", line_num, file_name);
                    free(word);
                    return ON;
                }
            }  
        }temp += i;
        free(word);
        if(*temp != '\0' && is_comma == OFF){
            print_error("Missing a comma", line_num, file_name);
            return ON;
        }
        word = get_word(temp, allocation_error);
        if(*allocation_error == ON) return ON;
        if(word == NULL && is_comma == ON){
            print_error("Illegal comma", line_num, file_name);
            return ON;
        }
    }
    
    return OFF;
}

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
flag check_string_instruction(char *line, int line_num, char *file_name){
    char *start, *end;
    for(start = line; *start != '\0' && (*start == ' ' || *start == '\t' || *start == ','); start++){
            if(*start == ','){
                print_error("Illegal comma", line_num, file_name);
                return ON;
            }
    }
    for(end = start+1; *end != '\0' && *end != '"'; end++);

    if(*start != '"' || *end != '"'){
        print_error("Not a string", line_num, file_name);
    }
    return OFF;
}

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
flag check_entry_instruction(list *command_list, char *line, int line_num, char *file_name, flag *allocation_error){
    flag is_comma = OFF;
    char *word = NULL;
    list *temp;

    for(; *line != '\0' && (*line == ' ' || *line == '\t' || *line == ','); line++){
        if(*line == ','){
            print_error("Illegal comma", line_num, file_name);
            return ON;
        }
    }

    word = get_word(line, allocation_error);
    if(*allocation_error == ON) return ON;
    while(word != NULL){
        temp = list_search(command_list, word);
        if(temp != NULL && return_type(temp) == EXTERN){
            print_error("Dfeined as extern and entry at the same file", line_num, file_name);
            free(word);
            return ON;
        }
        if(check_label(word) == -1){
            free(word);
            print_error("Invalid_lable", line_num, file_name);
            return ON;
        }
        line += strlen(word);
        free(word);
        is_comma = OFF;
        for(; *line != '\0' && (*line == ' ' || *line == '\t' || *line == ','); line++){
            if(*line == ','){
                if(is_comma == OFF) is_comma = ON;
                else{
                    print_error("Illegal comma", line_num, file_name);
                    return ON;
                }
            }  
        }
        if(*line != '\0' && is_comma == OFF){
            print_error("Missing a comma", line_num, file_name);
            return ON;
        }
        word = get_word(line, allocation_error);
        if(*allocation_error == ON) return ON;
        if(word == NULL && is_comma == ON){
            print_error("Illegal comma", line_num, file_name);
            return ON;
        }
    }
    return OFF;
}

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
flag check_extern_instruction(list *command_list, char *line, int line_num, char *file_name, flag *allocation_error){
    flag is_comma = OFF;
    char *word = NULL;
    list *temp;

    for(; *line != '\0' && (*line == ' ' || *line == '\t' || *line == ','); line++){
        if(*line == ','){
            print_error("Illegal comma", line_num, file_name);
            return ON;
        }
    }

    word = get_word(line, allocation_error);
    if(*allocation_error == ON) return ON;
    while(word != NULL){
        if(check_label(word) == -1){
            print_error("Invalid label", line_num, file_name);
            free(word);
            return ON;
        }
        temp = list_search(command_list, word);
        
        if(temp != NULL){
            print_error("Dfeined as extern and local at the same file", line_num, file_name);
            free(word);
            return ON;
        }
        line += strlen(word);
        free(word);
        is_comma = OFF;
        for(; *line != '\0' && (*line == ' ' || *line == '\t' || *line == ','); line++){
            if(*line == ','){
                if(is_comma == OFF) is_comma = ON;
                else{
                    print_error("Illegal comma", line_num, file_name);
                    return ON;
                }
            }  
        }
        if(*line != '\0' && is_comma == OFF){
            print_error("Missing a comma", line_num, file_name);
            return ON;
        }
        word = get_word(line, allocation_error);
        if(*allocation_error == ON) return ON;
        if(word == NULL && is_comma == ON){
            print_error("Illegal comma", line_num, file_name);
            return ON;
        }
    }
    return OFF;
}

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
flag check_command_line(char *line, list *command_list, int line_num, char *file_name, flag *allocation_error){
    char *temp = line, *word = NULL;
    int command;
    flag error = OFF, is_comma = OFF;
    int command_vector[3] = {UNINITIALIZED, UNINITIALIZED, UNINITIALIZED};

    for(; *temp != '\0' && (*temp == ' ' || *temp == '\t'); temp++);
    word = get_word(temp, allocation_error);
    if(*allocation_error == ON) return ON;

    if((command = find_command(word)) < 0){
        print_error("Unknown command", line_num, file_name);
        free(word);
        return ON;
    }
    command_vector[0] = command;
    

    temp += strlen(word);
    free(word);
    for(; *temp != '\0' && (*temp == ' ' || *temp == '\t' || *temp == ','); temp++){
        if(*temp == ','){
            print_error("Illegal comma", line_num, file_name);
            return ON;
        }  
    }
    word = get_word(temp, allocation_error);
    if(*allocation_error == ON) return ON;

    if(word != NULL){
        
        if((command_vector[1] = find_addresing(word)) == -1){
            print_error("Unknowen addresing", line_num, file_name);
            return ON;
        }
        temp += strlen(word);
        free(word);
    }
    
    is_comma = OFF;
    for(; *temp != '\0' && (*temp == ' ' || *temp == '\t' || *temp == ','); temp++){
        if(*temp == ','){
            if(is_comma == OFF) is_comma = ON;
            else{
                print_error("Illegal comma", line_num, file_name);
                return ON;
            }
        }  
    }
    if(*temp != '\0' && is_comma == OFF){
        print_error("Missing a comma", line_num, file_name);
        return ON;
    }
    word = get_word(temp, allocation_error);
    if(*allocation_error == ON) return ON;
    
    if(word != NULL){
        if((command_vector[2] = find_addresing(word)) == -1){
            print_error("Unknowen addresing", line_num, file_name);
            return ON;
        }
        temp += strlen(word);
        free(word);
    }
    
    if(command_vector[1] != UNINITIALIZED && command_vector[2] == UNINITIALIZED){
        command_vector[2] = command_vector[1];
        command_vector[1] = UNINITIALIZED;
    }

    if(command_options(command_vector) == ON){
        print_error("Operands don't match the command", line_num, file_name);
        return ON;
    }

    for(; *temp != '\0'; temp++){
        if(!isspace(*temp)){
            print_error("Extra text at the end", line_num, file_name);
            return ON;
        }
    }
    return error;
}

/**
 * @brief Checks if there is mare aviable memory.
 *
 * @param nau the current memory size
 * @return ON if an the memory is full, otherwise OFF.
 */
flag check_memory(int num){
    if(num >= MEMORY_SIZE)
        return ON;
    else return OFF;
}

