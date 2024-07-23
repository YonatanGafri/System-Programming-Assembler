#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "ass_other.h"
#include "check_text.h"
#include "assembler.h"

void print_binary(int number, int numBits) {
    int bit, i;
    int groups[] = {4, 4, 4, 3};
    int groupIndex = 0;
    int bitCounter = 0;
    
    
    for (i = numBits - 1; i >= 0; i--) {
        bit = (number >> i) & 1;
        printf("%d", bit);
        bitCounter++;
        
        
        if (bitCounter == groups[groupIndex] && groupIndex < 3) {
            printf("-");
            bitCounter = 0;
            groupIndex++;
        }
    }
    printf("\n");
}

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
	FILE *new_file; 
    char *line = NULL, *temp, *word = NULL;
    int line_num = 1;
    int type;
    flag error = OFF;
    int IC = 0, DC = 0;
	char *new_file_name = add_postfix(name, ".ob", CHANGE, &allocation_error);
    list *data_image = create_new_list(&allocation_error);
	list *encoded_line = create_new_list(&allocation_error);
	if(allocation_error == ON) allocation_error_handle("llls", command_list, data_image, encoded_line, name);
	new_file = fopen(new_file_name, "w");

	printf("the input file is called %s\n", name);
	printf("the output file is called %s\n", new_file_name);

    if (!(file = fopen(name, "r"))) {
        print_error("Error: Could not open the file", UNINITIALIZED, name);
        fclose(new_file);
		free_list(data_image);
		free_list(encoded_line);
        return ON;
    }
    if (!new_file){
		print_error("Error: Could not create a new file", UNINITIALIZED, new_file_name);
		fclose(file);
		free_list(data_image);
		free_list(encoded_line);
        return ON;
	}

    while((line = fget_line(file, &allocation_error)) != NULL){
        temp = line;
        if(check_line_length(temp, line_num, name) == ON || check_lable_define(temp, command_list, line_num, name, &allocation_error) == ON){
			if(allocation_error == ON) allocation_error_handle("slll", line, data_image, encoded_line, command_list);
            free(line);
			free_list(data_image);
			free_list(encoded_line);
            error =  ON;
            continue;
        }
        for(; *temp != '\0' && (*temp == ' ' || *temp == '\t' || *temp == ','); temp++);
	
        word = get_word(temp, &allocation_error);
		if(allocation_error == ON) allocation_error_handle("llls", command_list, data_image, encoded_line, name); 
        if(word != NULL && word[strlen(word)-1] == ':') temp += strlen(word);
        free(word);

        if((type = classify_line(temp, &allocation_error)) == INSTRUCTIONAL){
			if(allocation_error == ON) allocation_error_handle("llls", command_list, data_image, encoded_line, name);
            if(check_instructional_line(command_list, temp, line_num, name, &allocation_error) == OFF){
                directive_line(line, &DC, command_list, data_image, &allocation_error);
            }
            else error = ON; 
			if(allocation_error == ON) allocation_error_handle("llls", command_list, data_image, encoded_line, name);
        }
        else{
            if(check_command_line(temp, command_list, line_num, name, &allocation_error) == OFF){
                command_line(line, &IC, command_list ,encoded_line, &allocation_error);
            }
            else error = ON;
			if(allocation_error == ON) allocation_error_handle("llls", command_list, data_image, encoded_line, name); 
        }

        line_num++;
        free(line);
    }
	if(allocation_error == ON) allocation_error_handle("llls", command_list, data_image, encoded_line, name);
    fclose(file);
    fclose(new_file);
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
	while(isspace(line[i]) || line[i] == ',') i++;
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
	printf("in get data func\n");
	while(isspace(line[i])) i++;
	while(line[i] != '\0'){

		num = get_number((line + i), &is_num_set, allocation_error);
		if(*allocation_error == ON) return;
		printf("%d == ", num);
		print_binary(num, 15);
		if(is_num_set == ON){
			add_to_list(data_image, NULL, *DC, num, allocation_error);
			if(*allocation_error == ON) return;
			(*DC)++;
		}

		/*Move the cursor to the next char which is not whitespace */
		while(!isspace(line[i])) i++;
		while(isspace(line[i]) || line[i] == ',') i++;
		
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
	while(isspace(line[i])) i++;

	while(!isspace(line[i]) && line[i] != '\0'){
		if(line[i] == '"'){
			i++;
			continue;
		}		
		code = line[i];
		printf(" char is %c ", line[i]);
		print_binary(code, 15);
		add_to_list(data_image, NULL, *DC, code, allocation_error);
		if(*allocation_error == ON) return;
		(*DC)++;
		i++;
	}
	add_to_list(data_image, NULL, *DC, 0, allocation_error); /* Null-Terminator ends the string, the '\0' signs that's not a label without address */
}

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
void get_labels(char *line ,list *data_image, int *DC , int label_type, flag *allocation_error){
	int i = 0;
	char *label;
	int sum =0;
	while(isspace(line[i])) i++;
	
	while(line[i] != '\0' && line[i] != '\n'){
		
		label = get_word(line+i, allocation_error);
		printf(" %s ", label);
		sum++;
		if(*allocation_error == ON) return;
		/*label[strlen(label) - 1] = '\0';*/ /*Remove ':' from the label */
		edit_or_add_to_list(data_image, label, *DC, label_type, allocation_error);
		if(*allocation_error == ON) return;
		++*DC;
		free(label);
	
		/*Move the cursor to the next char which is not whitespace */
		while (line[i] != '\0' && !isspace(line[i]) && line[i] != ',') i++;
        	while (isspace(line[i]) || line[i] == ',') i++;
	}
	printf("sum of labels - %d ", sum);
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
	printf("in directive line func\n");
	while(isspace(line[i])) i++;
	
	word = get_word(line+i, allocation_error);
	if(*allocation_error == ON) return;

	/*if the first word is a label, keep it and move to the instruction*/
	if(*(word + strlen(word) - 1) == ':'){
		word[strlen(word)] = '\0'; /*Remove ':' from the label */
		label = word;
		i += strlen(word);
		while(isspace(line[i])) i++;
		word = get_word(line+i, allocation_error);
		if(*allocation_error == ON) return;
		within_label = ON;
	}
	
	i += strlen(word);
	if(strcmp(word, ".data") == 0){ 
		if(within_label == ON){		
			edit_or_add_to_list(command_list, label, *DC, DATA, allocation_error);
			if(*allocation_error == ON){
				free(word);
				return;
			}
			get_data(line +i, DC, data_image, allocation_error);
			if(*allocation_error == ON){
				free(word);
				return;
			}
		}
	}
	else if(strcmp(word, ".string") == 0){
		if(within_label == ON){			
			edit_or_add_to_list(command_list, label, *DC, DATA, allocation_error);
			if(*allocation_error == ON){
				free(word);
				return;
			}
		}
		get_string(line+i, DC, data_image, allocation_error);
		if(*allocation_error == ON){
			free(word);
			return;
		}
	}
	else if(strcmp(word, ".extern") == 0){
		get_labels(line+i , command_list, DC, EXTERN, allocation_error);  
	}
	else if(strcmp(word, ".entry") == 0){ 
		get_labels(line+i , command_list, DC, ENTRY, allocation_error);
		 
	}
	free(word);
}

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
void command_line(char *line, int *DI, list *command_list, list *encoded_line, flag *allocation_error){
	
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
	int method;
	printf("in command line func\n");
	while(isspace(line[i])) i++;
	word = get_word(line + i, allocation_error);
	if(*allocation_error == ON) return;
	puts(line);
	if(word[strlen(word)] == ':'){/*word is label's name*/
		add_to_list(command_list, word, *DI , CODE, allocation_error); /*add to labels table*/
		if(*allocation_error == ON){
			free(word);
			return;
		}
		while(!isspace(line[i])) i++;/*move pointer to the next word - command*/
		while(isspace(line[i])) i++;
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
	
	switch(cmd){
		
		case rts:
		case stop:
			break;

		case mov:
		case cmp:
		case add:
		case sub:
		case lea:
	 		while(!isspace(line[i])) i++;/*move pointer to the next word - command*/
    			while(isspace(line[i])) i++;
			
			if(line[i] == '#' ) method = IMMEDIATE;
			else if(line[i] ==  '*'){ 
				method = INDIR_REG;
				i++;/*for extract the register's name*/
			}
			else method = LABEL;
			
			free(word);
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
				if(*allocation_error == ON){
					free(word);
					return;
				}
				source_operand_line = (num << SHIFT_NUM); 
				mask = (1 << TURN_ON_A);
				source_operand_line |= mask;
			
			
			}else if(method == LABEL){/*Indirect value -label */
				mask = (1 << TURN_ON_SRC_DIRECT);
				upcode_line |= mask;
			
				add_to_list(encoded_line, word, *DI, 0, allocation_error);
				if(*allocation_error == ON){
					free(word);
					return;
				}
				/*yet we do not know the address of it, thus source_operand_line is 0
				* in the second pass we update the ARE as well */


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
		   	method = OFF;
					/*move to the next word -- t a r g e t   o p e r a n d  */
			while(!isspace(line[i])) i++;
			while( isspace(line[i]) || line[i] == ',') i++;
			/*countiue to default case for target operand*/

	    	default:  
			if(line[i] == '#' ) method = IMMEDIATE;
			else if(line[i] ==  '*'){
				method = INDIR_REG;
				i++;
			}
			else method = LABEL;
			
			free(word);
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
			printf("\nmethod tgt operand is %d\n ", method);

			if(method == IMMEDIATE){ /*Immediate value*/
				mask = (1 << TURN_ON_TRGT_IMMEDIATE);
				upcode_line |= mask;
			
				num = get_number(line + i + 1, &num_flag, allocation_error);
				if(*allocation_error == ON){
					free(word);
					return;
				}
				target_operand_line = (num << SHIFT_NUM);
				mask = (1 << TURN_ON_A);
				target_operand_line |= mask;

			}else if(method == LABEL){/*it has reached to a label what if the lable is r10 !?*/
				mask = (1 << TURN_ON_TRGT_DIRECT);
				upcode_line |= mask;
				add_to_list(encoded_line, word, *DI, 0, allocation_error); /*Yet we do not know the address of it, thus target_operand_line is 0 */
				if(*allocation_error == ON){
					free(word);
					return;
				}

			}else if(method == INDIR_REG){/*indirect register address*/
				mask = (1 << TURN_ON_TRGT_INDIRECT_REG);
				upcode_line |= mask;
				tgt_indirect_reg = ON;			
			}else {/*method == direct register address*/
				printf(" find direct target register ! ! ");
				mask = (1 << TURN_ON_TRGT_DIRECT_REG);
				upcode_line |= mask;
				tgt_direct_reg = ON;	
			}
			printf(" the char is %c  ",line[i]);
			printf("\nword is %s \n", word);
			
			for(reg = 0; reg < SUM_REGISTERS; reg++){
				if(strcmp(word,list_of_reg[reg]) == 0){
					
					mask = (reg << SHIFT_TARGET_LOCATION); 
					if((src_indirect_reg == ON && tgt_indirect_reg == ON) || (src_direct_reg == ON  && tgt_direct_reg == ON)){
						printf("same regiters types \n");
						source_operand_line |= mask;/* Two same registers so one line for both */
						mask = (1 << TURN_ON_A);
						source_operand_line |= mask;
					}else{
						printf("different types registers\n");
						target_operand_line = mask;
						mask = (1 << TURN_ON_A);
						target_operand_line |= mask;
					}				
				}
			}
	}
	
	free(word);
		/* Send all the lines to their address and update DI */
	mask = (1 << TURN_ON_A);
	upcode_line |= mask;
	printf("upcde-src -trgt -ARE\n");
	print_binary(upcode_line, 15);
	add_to_list(encoded_line, NULL, upcode_line, *DI, allocation_error);
	if(*allocation_error == ON) return;
	
	if(source_operand_line){
		printf("source operand line is - ");
		print_binary(source_operand_line, 15);
		add_to_list(encoded_line,NULL, ++*DI, source_operand_line, allocation_error);
		if(*allocation_error == ON) return;
	}
	if(target_operand_line){
		printf("target operand line is - ");
		print_binary(target_operand_line, 15);		
		add_to_list(encoded_line,NULL, ++*DI, target_operand_line, allocation_error);
		if(*allocation_error == ON) return;
	}		
}


void second_assembler(char *name, list *command_list){

}
