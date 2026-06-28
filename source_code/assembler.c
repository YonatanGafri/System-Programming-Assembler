#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "ass_other.h"
#include "check_text.h"
#include "assembler.h"
#include "second_assembler.h"




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
flag first_assembler(char *name, list *command_list){
    FILE *file;
	flag allocation_error = OFF;
    char *line = NULL, *temp, *word = NULL;
    int line_num = 1;
    int type;
    flag error = OFF;
    int IC = START_POINT, DC = 0;
    list *data_image = create_new_list(&allocation_error);
	list *encoded_line = create_new_list(&allocation_error);
	list *curent = NULL;

	if(allocation_error == ON) allocation_error_handle("llls", command_list, encoded_line, data_image); /*exit and free allocated memory*/

    if (!(file = fopen(name, "r"))) {
        print_error("Error: Could not open the file", UNINITIALIZED, name);
        /*fclose(new_file);*/
		free_list(data_image);
		free_list(encoded_line);
        return ON;
    }

    while((line = fget_line(file, &allocation_error)) != NULL){
        temp = line;
		/*check the line length, memry avable in the system and if theres a lable definition - checks if its good
																			!!!assuming that a lable can also have a '_' */
        if(check_line_length(temp, line_num, name) == ON || check_lable_define(temp, command_list, line_num, name, &allocation_error) == ON || check_memory(DC + IC) == ON){
			if(allocation_error == ON) allocation_error_handle("sslll", line, name, data_image, encoded_line, command_list);
            free(line);
            error =  ON;
			line_num++;
            continue;
        }
        for(; *temp != '\0' && (*temp == ' ' || *temp == '\t' || *temp == ','); temp++);
	
        word = get_word(temp, &allocation_error);
		if(allocation_error == ON) allocation_error_handle("sslll", line, name, data_image, encoded_line, command_list); /*if the first word is a label skip to the nex word*/
        if(word != NULL && word[strlen(word)-1] == ':') temp += strlen(word);
        free(word);
		

        if((type = classify_line(temp, &allocation_error)) == INSTRUCTIONAL){ /*the line from the type .data or .string an so on*/
			if(allocation_error == ON) allocation_error_handle("sslll", line, name, data_image, encoded_line, command_list); /*exit and free allocated memory*/
            if(check_instructional_line(command_list, temp, line_num, name, &allocation_error) == OFF){ /*check the line for errors*/
                directive_line(line, &DC, command_list, data_image, &allocation_error); /*save the lines data*/
            }
            else error = ON; 
			if(allocation_error == ON) allocation_error_handle("sslll", line, name, data_image, encoded_line, command_list); /*exit and free allocated memory*/
        }
        else if (type == COMMAND){ /*the line is a command line*/
            if(check_command_line(temp, command_list, line_num, name, &allocation_error) == OFF){ /*check the line for errors*/
                command_line(line, &IC, command_list ,encoded_line, &allocation_error); /*save the data*/
            }
            else error = ON;
			if(allocation_error == ON) allocation_error_handle("sslll", line, name, data_image, encoded_line, command_list); /*exit and free allocated memory*/
        }
        line_num++;
        free(line);
    }
	if(allocation_error == ON) allocation_error_handle("llls", command_list, data_image, encoded_line, name); /*exit and free allocated memory*/
    fclose(file);

	if(return_name(encoded_line) == NULL && return_value(encoded_line) == UNINITIALIZED && return_type(encoded_line) == UNINITIALIZED){
		free_list(encoded_line);
		encoded_line = NULL;
	}

	if(return_name(data_image) == NULL && return_value(data_image) == UNINITIALIZED && return_type(data_image) == UNINITIALIZED){
		free_list(data_image);
		data_image = NULL;
	}

	for(curent = data_image; curent != NULL; curent = return_next(curent)){ /*update the data counter to be after the command counter*/
		change_type(curent, return_type(curent) + IC);
	}
	for(curent = command_list; curent != NULL; curent = return_next(curent)){ 
		if(return_type(curent) == DATA || return_type(curent) == DATA_ENTRY)
			change_value(curent, return_value(curent) + IC);
	}

	if(error == OFF){
		if(second_assembler(name, command_list, encoded_line) == ON){
			free_list(encoded_line);
			free_list(data_image);	
			return ON; /* send to the second file reading*/
		}
	}


	if(error == OFF) print_output_files(name, encoded_line, data_image, command_list); /*print in to files*/

	free_list(encoded_line);
	free_list(data_image);

    return error;
}

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
int get_number(char *line, flag *is_num_set, flag *allocation_error){/*Extract numbers for .data and for immediate operand*/
	char *token = NULL;
	size_t tokenLen = 0;
	int num;
	int i = 0;
	while(line[i] != '\0' && (isspace(line[i]) || line[i] == ',')) i++;
	while(1){
		/*It has reaced to te end of line */
		if(line[i] == ',' || isspace(line[i]) || line[i] == '\0'){
			if (token == NULL || tokenLen == 0){
				/* there is no more numbers */
				*is_num_set = OFF;
				return 0;
			
			}else if (token != NULL && tokenLen > 0) { /* Null-terminate the string */
                token = (char *)realloc(token, tokenLen + 1);
                if (token == NULL) {
                   	*allocation_error = ON;
					return 0;
                }
                token[tokenLen] = '\0';

			}
			/* flag that signs there is another number */
            *is_num_set = ON;
            			/* Convert token to integer and return */
			num = atoi(token);
			free(token);
			return num;
		}
		token = (char *)realloc(token, tokenLen + 1);
        if (token == NULL) {
            *allocation_error = ON;
			return 0;
        }
		
        token[tokenLen++] = line[i];
		i++;
		continue;		
	}
	free(token);
}

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
void get_data(char *line, int *DC, list *data_image, flag *allocation_error){
	
	int i = 0;
	flag is_num_set = OFF;
	int num;
	
	while(line[i] != '\0' && (isspace(line[i]) || line[i] == ',')) i++;
	while(line[i] != '\0'){

		num = get_number((line + i), &is_num_set, allocation_error);
		if(*allocation_error == ON) return;
		if(is_num_set == ON){
			add_to_list(data_image, NULL, num, *DC, allocation_error);
			if(*allocation_error == ON) return;
			(*DC)++;
		}

		/*Move the cursor to the next char which is not whitespace */
		while (line[i] != '\0' && !isspace(line[i]) && line[i] != ',') i++;
        while (line[i] != '\0' && (isspace(line[i]) || line[i] == ',')) i++;
		
		is_num_set = OFF;
	}	
}

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
void get_string(char *line, int *DC, list *data_image, flag *allocation_error){
	int i = 0;
	int code; /*Store the encoded char */

	while(line[i] != '\0' && isspace(line[i])) i++;
	i++; /*skip first '"'*/
	while(line[i] != '\0' && line[i] != '"'){		
		code = line[i];
		add_to_list(data_image, NULL, code, *DC, allocation_error);
		if(*allocation_error == ON) return;
		(*DC)++;
		i++;
	}
	add_to_list(data_image, NULL, 0, *DC, allocation_error); /* Null-Terminator ends the string, the '\0' signs that's not a label without address */
	(*DC)++;
}

/**
 * @brief Processes labels in a line of text and adds them to the data image.
 *
 * This function reads a line of text containing labels, processes each label,
 * and adds them to the data image at the specified DC address.
 *
 * @param line The line of text containing the labels.
 * @param command_list Pointer to the list representing the symbole list.
 * @param label_type The type of the label (e.g., EXTERN, ENTRY).
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void get_labels(char *line ,list *command_list, int label_type, flag *allocation_error){
	int i = 0;
	char *label;
	list *temp;

	while(line[i] != '\0' && isspace(line[i])) i++;
	
	while(line[i] != '\0' && line[i] != '\n'){
		
		label = get_word(line+i, allocation_error);
		if(*allocation_error == ON) return;
		
		if(label_type == ENTRY){
			if((temp = list_search(command_list, label)) != NULL){
				if(return_type(temp) == DATA)
					change_type(temp, DATA_ENTRY);
				else if(return_type(temp) == CODE)
					change_type(temp, CODE_ENTRY);
			}
			else add_to_list(command_list, label, UNKNOWEN, label_type, allocation_error);
			if(*allocation_error == ON){
				free(label);
				return;
			}
		}
		else if(label_type == EXTERN)
			add_to_list(command_list, label, 0, label_type, allocation_error);
		free(label);
		if(*allocation_error == ON)	return;
		
		/*Move the cursor to the next char which is not whitespace */
		while (line[i] != '\0' && !isspace(line[i]) && line[i] != ',') i++;
        while (line[i] != '\0' && (isspace(line[i]) || line[i] == ',')) i++;
	}
}

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
void directive_line(char *line, int *DC, list *command_list, list *data_image, flag *allocation_error){/*we need handle both lists for either the label is command or directive */
	
	int i = 0;
	char *word =NULL;
	char *label = NULL;
	flag within_label = OFF;
	
	while(line[i] != '\0' && isspace(line[i])) i++;
	
	word = get_word(line+i, allocation_error);
	if(*allocation_error == ON) return;
	
	/*if the first word is a label, keep it and move to the instruction*/
	if(*(word + strlen(word) - 1) == ':'){
		*(word + strlen(word) - 1) = '\0'; /*Remove ':' from the label */
		label = word;
		i += (strlen(word)+1); /* +1 becuase of the aditional null terminator*/
		while(line[i] != '\0' && isspace(line[i])) i++;
		word = get_word(line+i, allocation_error);
		if(*allocation_error == ON){
			free(label);
			return;
		}
		within_label = ON;
	}
	
	i += strlen(word);
	if(strcmp(word, ".data") == 0){ 
		if(within_label == ON){	
			if(list_search(command_list, label) != NULL) /*make sure not to override an entry label*/
				edit_or_add_to_list(command_list, label, *DC, DATA_ENTRY, allocation_error);
			else
				add_to_list(command_list, label, *DC, DATA, allocation_error);

			if(*allocation_error == ON){
				free(word);
				free(label);
				return;
			}
		}
			get_data(line +i, DC, data_image, allocation_error);
			if(*allocation_error == ON){
				free(word);
				free(label);
				return;
			}
		
	}
	else if(strcmp(word, ".string") == 0){
		if(within_label == ON){			
			if(list_search(command_list, label) != NULL) /*make sure not to override an entry label*/
				edit_or_add_to_list(command_list, label, *DC, DATA_ENTRY, allocation_error);	
			else
				add_to_list(command_list, label, *DC, DATA, allocation_error);
			if(*allocation_error == ON){
				free(word);
				free(label);
				return;
			}
		}
		get_string(line+i, DC, data_image, allocation_error);
		if(*allocation_error == ON){
			free(word);
			free(label);
			return;
		}
	}
	else if(strcmp(word, ".extern") == 0){
		get_labels(line+i , command_list, EXTERN, allocation_error);  
	}
	else if(strcmp(word, ".entry") == 0){ 
		get_labels(line+i , command_list, ENTRY, allocation_error);
		 
	}
	free(word);
	free(label);
}

/**
 * @brief Processes a command line and updates the encoded line list.
 *
 * This function processes a command line, extracts the opcode and operands,
 * and updates the encoded line list with the corresponding binary representation.
 *
 * @param line The command line to process.
 * @param IC Pointer to the instruction counter.
 * @param command_list Pointer to the list of commands.
 * @param encoded_line Pointer to the list representing the encoded lines.
 * @param allocation_error Pointer to a flag that indicates if there was an allocation error.
 */
void command_line(char *line, int *IC, list *command_list, list *encoded_line, flag *allocation_error){
	
	char *list_of_commands[] = COMMAND_TABLE;
	char *list_of_reg[] = REG_TABLE; 
	
	int i = 0;
	int cmd, reg, num, mask;

	/*Store here the encoded lines respectively */
	int upcode_line = 0; 
	int source_operand_line = 0; 
	int target_operand_line = 0;

	/*Store the command and the operands strings*/
	char *word = NULL;
	
	/* Flags for check if there are two registers from the same type, so one line holds both */
	flag src_direct_reg = OFF, src_indirect_reg = OFF, tgt_direct_reg = OFF, tgt_indirect_reg = OFF;
	flag num_flag = ON; /*for pass to get num - there is no need in this func*/
	char *first_op_direct = NULL;
	char *second_op_direct = NULL;
	int method;
	
	while(line[i] != '\0' && isspace(line[i])) i++;
	word = get_word(line + i, allocation_error);
	if(*allocation_error == ON) return;
	if(*(word + strlen(word) - 1) == ':'){
		*(word + strlen(word) - 1) = '\0'; 
		if(list_search(command_list, word) != NULL)
			edit_or_add_to_list(command_list, word, *IC , CODE_ENTRY, allocation_error); /*add to labels table*/
		else
			edit_or_add_to_list(command_list, word, *IC , CODE, allocation_error);
		if(*allocation_error == ON){
			free(word);
			return;
		}
		while(line[i] != '\0' && !isspace(line[i])) i++;/*move pointer to the next word - command*/
		while(line[i] != '\0' && isspace(line[i])) i++;
		
		free(word);
		word = get_word(line + i, allocation_error);
		if(*allocation_error == ON) return;
	}
	
	/*Extract command code */
	for (cmd = 0; cmd < SUM_COMMANDS; cmd++) { /* Variable num runs over the list of commands and represents thier decimal code */
		if(strcmp(word, list_of_commands[cmd]) == 0){
			upcode_line |= (cmd << SHIFT_UPCODE);
			break;
		} 
	}	
	free(word);
	
	switch(cmd){
		
		case rts:
		case stop:
			
			break;

		case mov:
		case cmp:
		case add:
		case sub:
		case lea:
	 		while(line[i] != '\0' && !isspace(line[i])) i++;/*move pointer to the next word - command*/
    		while(line[i] != '\0' && isspace(line[i])) i++;
			
			if(line[i] == '#' ) method = IMMEDIATE;
			else if(line[i] ==  '*'){ 
				method = INDIR_REG;
				i++;/*for extract the register's name*/
			}
			else method = LABEL;
			
			word = get_word(line + i, allocation_error); /*extract the operand name */
			if(*allocation_error == ON) return;

			for(reg = 0; reg < SUM_REGISTERS; reg++){/*check the word weather is label or direct reg*/
				if(strcmp(word,list_of_reg[reg]) == 0){/*check if the operand is a label*/
					if(method != INDIR_REG){
						method = DIR_REG;
					}
					break;
				}
			}

			/* Extract soucre operand */
			if(method == IMMEDIATE){ /*Immediate value*/
				mask = (1 << TURN_ON_SRC_IMMEDIATE);
				upcode_line |= mask;

				num = get_number(line + i +1, &num_flag, allocation_error);
				if(*allocation_error == ON) return;
				
				source_operand_line = (num << SHIFT_NUM); 
				mask = (1 << TURN_ON_A);
				source_operand_line |= mask;
			
			
			}else if(method == LABEL){ /*Indirect value -label */
				mask = (1 << TURN_ON_SRC_DIRECT);
				upcode_line |= mask;
				first_op_direct = word;
				source_operand_line = 1;

			}else if (method == INDIR_REG){/*Indirect register address*/
				mask = (1 << TURN_ON_SRC_INDIRECT_REG);
				upcode_line |= mask;
				src_indirect_reg = ON;
				i++; /*move to the next char for extract the name of register */

			}else {/* method == DIR_REG*/
				mask = (1 << TURN_ON_SRC_DIRECT_REG);
				upcode_line |= mask;
				src_direct_reg = ON;
			}
	

			/*write code  line for the registers if there are*/
			if (src_direct_reg == ON || src_indirect_reg == ON){
				for(reg = 0; reg < SUM_REGISTERS; reg++){
					if(strcmp(word,list_of_reg[reg]) == 0){
						source_operand_line = (reg << SHIFT_SOURCE_LOCATION);
						mask = (1 << TURN_ON_A);
						source_operand_line |= mask;
						break;
					}
				}
			}
			if(method != LABEL) free(word); /*dont free if its a lable not to luse the label*/
		   	method = OFF;
				
					
			/*countiue to default case for target operand*/
	    default: 
			/*move to the next word -- t a r g e t   o p e r a n d  */
			while(line[i] != '\0' && !isspace(line[i])  && line[i] != ',') i++;
			while(line[i] != '\0' && (isspace(line[i]) || line[i] == ',')) i++;
			 
			if(line[i] == '#' ) method = IMMEDIATE;
			else if(line[i] ==  '*'){
				method = INDIR_REG;
				i++;
			}
			else method = LABEL;

			word = get_word(line + i, allocation_error); /*extract the operand name */
			if(*allocation_error == ON){
				free(first_op_direct);
				return;
			}
			
			for(reg = 0; reg < SUM_REGISTERS; reg++){/*check the word weather is label or direct reg*/
				if(strcmp(word,list_of_reg[reg]) == 0){/*check if the operand is a label*/
					if(method != INDIR_REG){					
						method = DIR_REG;
					}
					break;
				}
			}

			
			if(method == IMMEDIATE){ /*Immediate value*/
				mask = (1 << TURN_ON_TRGT_IMMEDIATE);
				upcode_line |= mask;
			
				num = get_number(line + i + 1, &num_flag, allocation_error);
				if(*allocation_error == ON){
					free(first_op_direct);
					free(word);
					return;
				}

				target_operand_line = (num << SHIFT_NUM);
				mask = (1 << TURN_ON_A);
				target_operand_line |= mask;

			}else if(method == LABEL){/*it has reached to a label */
				target_operand_line = 1;
				second_op_direct = word;
				target_operand_line = 1;
				mask = (1 << TURN_ON_TRGT_DIRECT);
				upcode_line |= mask;

			}else if(method == INDIR_REG){/*indirect register address*/
				mask = (1 << TURN_ON_TRGT_INDIRECT_REG);
				upcode_line |= mask;
				tgt_indirect_reg = ON;			
			}else {/*method == direct register address*/
				mask = (1 << TURN_ON_TRGT_DIRECT_REG);
				upcode_line |= mask;
				tgt_direct_reg = ON;	
			}
			
			for(reg = 0; reg < SUM_REGISTERS; reg++){
				if(strcmp(word,list_of_reg[reg]) == 0){
					
					mask = (reg << SHIFT_TARGET_LOCATION); 
					if((src_indirect_reg == ON || src_direct_reg == ON) && (tgt_indirect_reg == ON  || tgt_direct_reg == ON)){
						source_operand_line |= mask;/* Two same registers so one line for both */
						mask = (1 << TURN_ON_A);
						source_operand_line |= mask;
					}else{
						target_operand_line = mask;
						mask = (1 << TURN_ON_A);
						target_operand_line |= mask;
					}				
				}
			}
		if(method != LABEL) free(word);
	}

		/* Send all the lines to their address and updateIC */
	mask = (1 << TURN_ON_A);
	upcode_line |= mask;
	
	add_to_list(encoded_line, NULL, upcode_line, UNINITIALIZED, allocation_error);
	if(*allocation_error == ON){
		free(first_op_direct);
		free(second_op_direct);
		return;
	}
	
	
	if(source_operand_line){
		++(*IC);
		if(first_op_direct != NULL)
			add_to_list(encoded_line, first_op_direct, UNKNOWEN, USE_LABEL, allocation_error);
		else
			add_to_list(encoded_line, NULL, source_operand_line, UNINITIALIZED, allocation_error);
		if(*allocation_error == ON){
			free(first_op_direct);
			free(second_op_direct);
			return;
		}
	}
	if(target_operand_line){
		++(*IC);
		if(second_op_direct != NULL)
			add_to_list(encoded_line, second_op_direct, UNKNOWEN, USE_LABEL, allocation_error);		
		else
			add_to_list(encoded_line, NULL, target_operand_line, UNINITIALIZED, allocation_error);
		if(*allocation_error == ON){
			free(first_op_direct);
			free(second_op_direct);
			return;
		}
	}
	free(first_op_direct);
	free(second_op_direct);
	++(*IC);/*for the next line*/	
}

