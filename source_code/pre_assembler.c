/* 
 * pre_assembler.c
 * 
 * This program preprocesses an assembler file by expanding macros defined within it.
 * It reads an input assembler file, identifies macro definitions, and replaces instances
 * of those macros with their expanded definitions in a new output file.
 */

#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "ass_other.h"
#include "pre_assembler.h"

struct Macro{
    char *macr_name; 
    char **lines; /* Array of strings to store lines from the buffer */
    int num_lines; /* Number of lines */
    struct Macro *next;
};


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
flag pre_assembler(const char* filename, list *command_list){
	FILE *fd;
	char *word = NULL; /*Max string related to macro definition */
	char *name = NULL;/*Max size of name of macro*/
	char *ptr;
	int i, j = 0;
	Macro* current = NULL;
	Macro* prev = NULL;
	Macro* new_macro = NULL;
	Macro* firstMacroPointer = NULL;
	int sum_macros = 0;
	flag inside_macro = OFF;
	flag go_next_line = OFF;
	flag allocation_error = OFF;
	char buffer[MAX_LINE_LENGTH]; /*Size was declared in the header file*/
    	
	
	/* Add postfix for the output file */
	char* expanded_file_name = add_postfix(filename, ".am", CHANGE, &allocation_error);
	FILE *expanded_file = fopen(expanded_file_name, "w"); 
	if(allocation_error == ON) allocation_error_handle("ls", command_list, name);
	printf("the input file is called %s\n", filename);
	printf("the output file is called %s\n", expanded_file_name);
	
	
	/*--------------------------------------------------*/
	/*Check opening of both files*/
	if (!(fd = fopen(filename, "r"))) {
        print_error("Error: Could not open the file", UNINITIALIZED, filename);
		fclose(expanded_file);
        return ON;
    }
	
	/* Send to check the file for any error, and return true if there is an error */
	if (pre_assembler_check(fd, command_list, filename) == ON) { 
        fclose(fd);
		remove(expanded_file_name);
        return ON;
    }
	
	/*--------------------------------------------------*/
	if (!expanded_file){
		print_error("Error: Could not create a new file", UNINITIALIZED, expanded_file_name);
		fclose(fd);
        return ON;
	}
	
	
	/*--------------------------------------------------*/
	
	while (fgets(buffer, MAX_LINE_LENGTH, fd)){
		ptr = buffer;
		i = 0;
		while(isspace(ptr[i])) i++;
		if (ptr[i] == '\0' || ptr[i] == '\n' || ptr[i] == ';') continue; /*Either empty or comment line, continue to the next line*/
		free(word);
		word = get_word(ptr+i, &allocation_error);
		if(allocation_error == ON) allocation_error_handle("lssm", command_list, name, word, firstMacroPointer);
		
		if(strcmp(word, "macr") == 0){/*Decalre a new macro*/
			inside_macro = ON;
			/*go to the name of macro*/
			while(!isspace(ptr[i])) i++;
			while(isspace(ptr[i])) i++;
			free(name);
			name = get_word(ptr+i, &allocation_error);
			if(allocation_error == ON) allocation_error_handle("lssm", command_list, name, word, firstMacroPointer);
			add_to_list(command_list, name, UNINITIALIZED, MACRO, &allocation_error); /* Add the macro to the command list */
			if(allocation_error == ON) allocation_error_handle("lssm", command_list, name, word, firstMacroPointer);
			printf("word - %s,  name  %s\n", word, name);

			if (new_macro != NULL){/*We have already and macro*/
				prev = new_macro;
			}
			new_macro = create_macro(name, &allocation_error);
			if(allocation_error == ON) allocation_error_handle("lssm", command_list, name, word, firstMacroPointer);
			if(prev != NULL){
				prev->next = new_macro;
			}
			sum_macros++;
			if(sum_macros == 1){
				firstMacroPointer = new_macro;
			}
			continue; /*go to the next line for copying to the macro struct*/					
		}else if(strcmp(word, "endmacr") == 0){
			inside_macro = OFF;
			j = 0;
			continue; /*go to the next line*/
		}else {
			current = firstMacroPointer;
			while(current != NULL){
				if(strcmp(word, current->macr_name) == 0){
					for (i = 0; i < current->num_lines; ++i) {
        					fprintf(expanded_file, "%s", current->lines[i]);
    						
					}
					go_next_line = ON; /*Stopped copy the macro's body, so continue*/
					break;/* Exit the while loop once the macro is found */
				}else {
					current = current->next;
				}
			}		
		}
		if(go_next_line == ON){
			go_next_line = OFF;
			continue;
		}

		
		if(inside_macro == ON){
			add_line_to_macro(new_macro, buffer, &allocation_error);
			if(allocation_error == ON) allocation_error_handle("lssmm", command_list, name, word, firstMacroPointer, new_macro);
			j++;
		}else { /*inside_macro == OFF, copy each line*/
			fputs(buffer, expanded_file);
		}
	}
	
	free(word);
	free(name);
	
	free_macro(firstMacroPointer);	
	fclose(fd);
	/*assembler(expanded_file_name);*/	/*Send to the assembler*/
    fclose(expanded_file);
	printf("files were closed\n");
	
	return OFF;
}

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
Macro* create_macro(const char* name, flag *allocation_error){
	Macro* new_macro;

	new_macro = (Macro*)malloc(sizeof(Macro));
	if (new_macro == NULL) {
        *allocation_error = ON;
		return NULL;
    }
	new_macro->macr_name = (char*)malloc(strlen(name) + 1);
	if (new_macro->macr_name == NULL) {
        *allocation_error = ON;
		return NULL;
    }
	strcpy(new_macro->macr_name ,name);
	new_macro-> next = NULL;
	new_macro->num_lines = 0;
	new_macro->lines = (char**)malloc(MAX_MACRO_LINES * sizeof(char*)); 
	if (new_macro->lines == NULL) {
    	*allocation_error = ON;
		return NULL;
	}
	return new_macro;
}/*--------------------------------------------------------------------------------*/

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
void add_line_to_macro(Macro *macro, const char *line, flag *allocation_error) {
	int new_capacity;
	int max_macro_lines = 10;
	char** temp = NULL;
    if (macro->num_lines >= MAX_MACRO_LINES) {
        /* If more lines are needed, reallocate memory */
        new_capacity = 2 * max_macro_lines; /* Double the capacity */
        temp = (char**)realloc(macro->lines, new_capacity * sizeof(char*));
        if (temp == NULL) {
            *allocation_error = ON;
			return;
        }
        macro->lines = temp;
        max_macro_lines = new_capacity; /* Update the capacity */
    }

    /* Allocate memory for the new line and copy it */
    macro->lines[macro->num_lines] = (char*)malloc(strlen(line) + 1);
    if (macro->lines[macro->num_lines] == NULL) {
        *allocation_error = ON;
		return;
    }
    strcpy(macro->lines[macro->num_lines], line);
    macro->num_lines++;
}

/**
 * @brief Frees the memory allocated for the specified macro structure.
 *
 * This function frees the memory allocated for the specified macro structure and all its associated lines.
 *
 * @param current Pointer to the macro structure to free.
 */
void free_macro(Macro* current){
	Macro* temp;
	int i;
	while(current != NULL){
		temp = current;
		current = current->next;
		for(i = 0; i < temp->num_lines; i ++){
			free(temp->lines[i]);
		}
		free(temp->lines); /*Free the array of pointers*/
		free(temp->macr_name);
		free(temp);
	}
}
/*----------------------------------------------------------------------------------------------*/
	


/**
 * @brief Performs pre-processing checks on the input file.
 *
 * This function performs pre-processing checks on the input file to identify
 * macro definitions, ensure correct syntax, and detect errors. 
 * The function does not change the file pointer.
 *
 * @param file Pointer to the input file to be processed.
 * @param command_list Pointer to the list of commands.
 * @param file_name The name of the file being processed.
 * @return OFF if no errors were found during the check, otherwise returns ON.
 */
flag pre_assembler_check(FILE *file, list *command_list, const char *file_name){
    flag new_line = ON;             /* Flag to track if we are at the start of a new line */
    flag there_is_macro = OFF;      /* Flag to track if we are inside a macro definition */
    flag there_is_error = OFF;      /* Flag to indicate if any errors were encountered */
	flag allocation_error = OFF;
    char *word = NULL, *temp_line = NULL;                     /* Pointer to store the current word read from the file */
    int line = 1;                   /* Line number in the file being processed */
    long start_pos = ftell(file), temp;   /* Starting position in the file */

	
	while((temp_line = fget_line(file, &allocation_error)) != NULL){
		if(temp_line == NULL) allocation_error_handle("sl", temp_line, command_list);
		if(strlen(temp_line) > MAX_LINE_LENGTH){
			print_error("To many characters", line, file_name);
			there_is_error = ON;
		}
		line++;
		free(temp_line);
	}
	fseek(file, start_pos, SEEK_SET); /* Reset file pointer to the starting position */
	line = 1;

    while (!feof(file)) { /* Loop through the file until EOF is reached */
        while ((word = fget_word(file, &allocation_error)) != NULL) {
			if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
            if (strcmp(word, "macr") == 0 && new_line == OFF) { /* Check for illegal macro definition at the beginning of a line */
                print_error("Invalid macro definition", line, file_name);
                there_is_macro = ON;
                there_is_error = ON;
                do{free(word);} while ((word = fget_word(file, &allocation_error)) != NULL);
				if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
                break;
            }
            if (strcmp(word, "macr") == 0) { /* Process 'macr' keyword */
                if (there_is_macro == ON) { /* Check for nested macro definition */
                    print_error("Nested macro", line, file_name);
                    there_is_error = ON;
                    do{free(word);} while ((word = fget_word(file, &allocation_error)) != NULL);
					if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
                    break;
                } else {
                    there_is_macro = ON; /* Mark that we are inside a macro definition */
                    free(word);
                    word = fget_word(file, &allocation_error);
					if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
					/* Check if the macro name is valid (not already a command or list item) */
                    if (list_search(command_list, word) != NULL || find_command(word) != -1 || check_label(word) == -1 || find_register(word) != -1) { 
                        print_error("Invalid macro name", line, file_name);
                        there_is_error = ON;
                        do{free(word);} while ((word = fget_word(file, &allocation_error)) != NULL);
						if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
                        break;
                    }
					temp = ftell(file);
					free(word);
                    word = fget_word(file, &allocation_error); /* Check for illegal text after macro definition */
					if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
					fseek(file, temp, SEEK_SET);
                    if (word != NULL) {
                        print_error("Illegal text after macro", line, file_name);
                        there_is_error = ON;
                        do{free(word);} while ((word = fget_word(file, &allocation_error)) != NULL);
						if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
                        break;
                    }
                }
            }
            else if (strcmp(word, "endmacr") == 0) { /* Process 'endmacr' keyword */
                if (there_is_macro == OFF || new_line == OFF) {      /* Check if 'endmacr' appears outside a macro definition */
                    print_error("Illegal macro ending", line, file_name);
					there_is_macro = OFF;
                    there_is_error = ON;
                    do{free(word);} while ((word = fget_word(file, &allocation_error)) != NULL);
					if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
                    break;
                }
                there_is_macro = OFF; /* Mark that we are no longer inside a macro definition */
                free(word);

                temp = ftell(file);
                word = fget_word(file, &allocation_error); /* Check for illegal text after macro definition */
				if(allocation_error == ON) allocation_error_handle("sl", word, command_list);
				fseek(file, temp, SEEK_SET);

                if (word != NULL) {
                    print_error("Illegat text after macro", line, file_name);
                    there_is_error = ON;
                    do{free(word);} while ((word = fget_word(file, &allocation_error)) != NULL);
					if(allocation_error == ON) return ON;
                    break;
                }
            }
            free(word); /* Free memory allocated for the current word */
			new_line = OFF; /* We are no longer at the start of a new line */
        }
        new_line = ON; /* Mark that we are at the start of a new line */
        line++; /* Increment the line counter */
    }
    if (there_is_macro == ON) {  /* Check if there was a macro definition without an ending 'endmacr' */
        print_error("Missing macro ending", UNINITIALIZED, file_name);
        there_is_error = ON;
    }
    fseek(file, start_pos, SEEK_SET); /* Reset file pointer to the starting position */

    return there_is_error; /* Return flag indicating if any errors were found */
}