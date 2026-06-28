#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "pre_assembler.h"

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
char* add_postfix(const char *filename, const char *postfix, int flags, flag *allocation_error){
	int length;
	char* new_name = NULL;
	char* dotIndex;
	if(flags == ADD){ 
		length = strlen(filename) + strlen(postfix) + 1; /*Calculate the overall length of both*/
		new_name = (char*) malloc(length);
		if(new_name == NULL){
			*allocation_error = ON;
		    return NULL;
		}
		strcpy(new_name, filename);
		strcat(new_name, postfix);

	}else if(flags == CHANGE){/*Change the postfix*/
		dotIndex = strchr(filename, '.');
		length = dotIndex - filename + strlen(postfix) + 1;
		new_name = (char*) malloc(length);
		if(new_name == NULL){
			*allocation_error = ON;
		    return NULL;
		}
		
		strncpy(new_name, filename, dotIndex - filename);
		new_name[dotIndex- filename] = '\0';
		strcat(new_name, postfix); 
	}
	return new_name;
}

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
char* fget_word(FILE *file, flag *allocation_error){
    int size = 0, i;
    char c, *word;
    long start_pos;  /* Store the initial position */

    while((c = fgetc(file)) != EOF && (isspace(c) || c == ',')){ /*move the file pointer to the next word skiping white spaces and commas*/
        if (c == '\n') {
            return NULL;
        }
    }
        
    if(c == EOF) /*its the end of the file*/
        return NULL;

    fseek(file, -1, SEEK_CUR); /*move back 1 step*/
    start_pos = ftell(file); /*save this location*/
    
    /* Read characters from the file until a delimiter is encountered to determine the word length */
    while ((c = fgetc(file)) != EOF && !isspace(c) && c != ',' && c != '\n') {
        size++;
    }
    if (size == 0) {   /*If no word was found, return NULL*/ 
        return NULL;
    }
    word = (char*)malloc(sizeof(char) * (size + 1));
    if (word == NULL) { /*allocation error*/
        *allocation_error = ON;
		return NULL;
    }
    fseek(file, start_pos, SEEK_SET); /*Reset the file position to the start of the word*/
    for (i = 0; i < size; i++) {
        word[i] = fgetc(file);
    }
    word[size] = '\0';  /*Null-terminate the string*/
    return word;
}

/**
 * @brief Reads a line from the file and returns it.
 *
 * This function reads a line from the file and returns it.
 *
 * @param file The file pointer from which to read the line.
 * @param allocation_error A pointer to a flag that indicates if there was an allocation error.
 * @return A pointer to the dynamically allocated line, or NULL if no line is found. The caller is responsible for freeing the memory allocated for the word.
 */
char* fget_line(FILE *file, flag *allocation_error){
    int size = 0;
    char *line, c;
    long start_pos  = ftell(file);  /* Store the initial position */
    
    /* Read characters from the file until a delimiter is encountered to determine the line length */
    while ((c = fgetc(file)) != EOF && c != '\n') {
        size++;
    }
    if (c == EOF && size == 0) {   /*If no line was found, return NULL*/ 
        return NULL;
    }    
    fseek(file, start_pos, SEEK_SET); /*Reset the file position to the start of the line*/
    line = (char*)malloc(sizeof(char) * (size + 2));
    if (line == NULL) {
        *allocation_error = ON;
		return NULL;
    }
    fgets(line, size + 2, file);
    if (line[size] == '\n') { /*fgets sodnt erase the '\n'... */
        line[size] = '\0';
    }
    return line;
}


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
void print_error(char *error, int line, const char *file_name){
    printf("In file %s: ", file_name);

    line >= 0 ? printf("in line %d\n\t", line) : printf("\n\t");

    printf("Error! %s\n", error);
}

/**
 * @brief Retrieves the next word from a pointer.
 *
 * This function retrieves the next word from the provided pointer.
 *
 * @param ptr The pointer to the string from which to retrieve the word.
 * @param allocation_error A pointer to a flag that indicates if there was an allocation error.
 * @return A pointer to the dynamically allocated word, or NULL if no word is found. The caller is responsible for freeing the memory allocated for the word.
 */
char *get_word(char *ptr, flag *allocation_error) {
    char *word = NULL;
    char* temp = NULL;
    int word_length = 0;
    int capacity = 10; /* Initial capacity for word buffer */

    /* Allocate memory for the word buffer */
    word = (char *)malloc(capacity * sizeof(char));
    if (word == NULL) {
        *allocation_error = ON;
		return NULL;
    }

    /* Loop through characters until a space or NULL character is encountered */
    while (*ptr != '\0' && !isspace(*ptr) && *ptr != ',') {
        /* Check if we need to expand the buffer */
        if (word_length >= capacity - 1) {
            capacity *= 2; /* Double the capacity */
            temp = realloc(word, capacity * sizeof(char));
            if (temp == NULL) {
                free(word);
                *allocation_error = ON;
		        return NULL;
            }
            word = temp;
        }

        /* Store the current character in the word buffer */
        word[word_length++] = *ptr++;
    }

    /* Add null terminator to end the word string */
    word[word_length] = '\0';

    if(strlen(word) == 0){
        free(word);
        return NULL;
    }

    return word;
}

/**
 * @brief Handles allocation errors and frees allocated memory.
 *
 * This function handles allocation errors by freeing allocated memory and printing an error message and exiting the program.
 *
 * @param format A format string indicating the types of arguments ('s' for string, 'l' for list, 'm' for macro).
 * @param ... The arguments to be freed.
 */
void allocation_error_handle(char *format, ...){
    int args_num = strlen(format);
    int i;
    char *string;
    list *new_list;
    Macro *new_macro;

    va_list args;
    va_start(args, format);

    for(i = 0; i < args_num; i++){
        if(format[i] == 's'){
            string = va_arg(args, char*);
            free(string);
        }
        else if(format[i] == 'l'){
            new_list = va_arg(args, list*);
            free_list(new_list);
        }
        else if(format[i] == 'm'){
            new_macro = va_arg(args, Macro*); 
            free_macro(new_macro);
        }
    }
    printf("Allocation error!\n");
    exit(1);
}

/* 
 * Function: find_word_in_file
 * ---------------------------
 * This function searches for a given word in a file.
 * If the word is found, it returns the line number where the word is found.
 * If the word is not found or an error occurs, it returns -1.
 */

int find_word_in_file(char *filename, char *word) {
    FILE *file;  
    char line[MAX_LINE_LENGTH];  
    int line_num = 0;  
    flag found = OFF;  /* Flag to indicate if the word is found */

    file = fopen(filename, "r");  
    if (file == NULL) {  
        print_error("Error opening file", UNINITIALIZED, filename);  
        return -1;  
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line_num++;  
        if (strstr(line, word) != NULL) {  
            found = ON;  
            break;  
        }
    }

    fclose(file);  

    if (found)
        return line_num;  
    else return -1;
}
