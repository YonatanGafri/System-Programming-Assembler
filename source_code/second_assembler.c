#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "second_assembler.h"

/**
 * @brief Performs the second pass of the assembler.
 *
 * This function processes the encoded lines and updates the addresses of the labels.
 * If a label is not declared or declared as entry - but not declared local, it will print an error message.
 * The function modifies the values of the encoded lines based on the label addresses.
 *
 * @param file_name The name of the file being processed.
 * @param command_list The list of commands (labels) and their attributes.
 * @param encoded_line The list of encoded lines to be updated.
 * @return OFF if no errors were found, otherwise returns ON.
 */
flag second_assembler(char *file_name, list *command_list, list *encoded_line){
	list *cur;
	flag error = OFF;
	list *temp;

	for(cur = command_list; cur != NULL; cur = return_next(cur)){
		if(return_type(cur) == ENTRY){
			print_error("The varaible undeclaered", find_word_in_file(file_name, return_name(cur)), file_name);
			error = ON;
		}
	}

	for(cur = encoded_line; cur != NULL; cur = return_next(cur)){
		if(return_value(cur) == UNKNOWEN && return_type(cur) == USE_LABEL){ 
			if((temp = list_search(command_list, return_name(cur))) == NULL){ /*used a lablel thet was not declared*/
				print_error("The varaible undeclaered", find_word_in_file(file_name, return_name(cur)), file_name);
				error = ON;
			}else{
				change_value(cur, return_value(temp)); /*update the line to the value of the label*/
				if(return_type(temp) == EXTERN){
					change_value(cur, ((return_value(cur) << SHIFT_NUM) | TURN_ON_E));
				}
				else if(return_type(temp) == CODE_ENTRY || return_type(temp) == DATA_ENTRY || return_type(temp) == DATA || return_type(temp) == CODE){
					change_value(cur, ((return_value(cur) << SHIFT_NUM) | TURN_ON_R));
				}
			}
		}
	}
	return error;
}

/**
 * @brief Prints the output files for the assembler.
 *
 * This function generates the .ob, .ext, and .ent files based on the encoded lines and data image.
 * It prints the encoded lines and data lines to the .ob file, and any external and entry labels to the .ext and .ent files respectively.
 *
 * @param name The base name of the files being processed.
 * @param encoded_line The list of encoded lines.
 * @param data_image The list of data lines.
 * @param command_line The list of commands (labels) and their attributes.
 */
void print_output_files(char *name, list *encoded_line, list *data_image, list *command_line){
	
	list *cur, *temp;
	flag allocation_error = OFF;
	char *ob_file_name = add_postfix(name, ".ob",CHANGE, &allocation_error);
	char *ext_file_name = add_postfix(name, ".ext", CHANGE, &allocation_error);
	char *ent_file_name = add_postfix(name, ".ent", CHANGE, &allocation_error);
	int sum_instructions_lines = 0, sum_data_lines = 0;
	flag new_file_ext = OFF, new_file_ent = OFF;
	unsigned int code;
	FILE *ob_file = fopen(ob_file_name, "w"); 
	FILE *ext_file = NULL, *ent_file = NULL;
	int line_num = START_POINT;
	
	if (allocation_error == ON) allocation_error_handle("lllssss", encoded_line, data_image, command_line, name, ob_file_name, ext_file_name, ent_file_name);

	if (ob_file == NULL) {
        print_error("Error opening file!", UNINITIALIZED, ob_file_name);
		free(ob_file_name);
		free(ext_file_name);
		free(ent_file_name);
        return;
    }

	
	/*count the lines ant print them*/
	sum_instructions_lines = count_list(encoded_line);
	sum_data_lines = count_list(data_image);
	fprintf(ob_file, "%3d %3d\n",sum_instructions_lines, sum_data_lines);

	
	/*Write instructions lines in object file*/
	cur = encoded_line;
	while(cur){
		code = (return_value(cur) & 0xFFFF) % 0100000;/* Ensure the octal value is within 5 digits*/
		if (return_type(cur) == USE_LABEL){/*if encounter label, create file and write it*/
			if(return_type_symbol(return_name(cur), command_line) == EXTERN){
				if(new_file_ext == OFF){
					new_file_ext = ON;
					printf("There is an externs file\n");
					ext_file = fopen(ext_file_name, "w");
					if (ext_file == NULL) {
						print_error("Error opening file", UNINITIALIZED, ext_file_name);
						fclose(ob_file);
						if(new_file_ent == ON) fclose(ent_file);
						free(ext_file_name);
						free(ob_file_name);
						free(ent_file_name);
						return;
		    			}
				}
				fprintf(ext_file,"%s  %04d\n", return_name(cur), line_num);
			}
		}
		
		
		fprintf(ob_file,"%04d %05o\n", line_num,(unsigned int) code );
		cur = return_next(cur);
		sum_instructions_lines++;
		line_num++;
	}

	/*Write data lines in object file*/
	cur = data_image;
		while(cur){
		code = (return_value(cur)  & 0xFFFF) % 0100000;
		if (return_type(cur) == USE_LABEL){
			if(return_type_symbol(return_name(cur), command_line) == EXTERN){
				if(new_file_ext == OFF){
					new_file_ext = ON;
					printf("There is an externs file\n");
					ext_file = fopen(ext_file_name, "w");
					if (ext_file == NULL) {
						print_error("Error opening file", UNINITIALIZED, ent_file_name);
						fclose(ob_file);
						if(new_file_ent == ON) fclose(ext_file);
						free(ext_file_name);
						free(ob_file_name);
						free(ent_file_name);
						return;
		    			}
				}
				fprintf(ext_file,"%s  %04d\n", return_name(cur), line_num);
			}
		}
		fprintf(ob_file,"%04d %05o\n", line_num,(unsigned int) code );
		cur = return_next(cur);
		sum_data_lines++;
		line_num++;
	}
	
	temp = sort_list(command_line, &allocation_error);
	

	if(allocation_error == ON) allocation_error_handle("lllssss", encoded_line, data_image, command_line, name, ob_file_name, ext_file_name, ent_file_name);

	for(cur = temp; cur; cur = return_next(cur)){
		if(return_type(cur) == CODE_ENTRY || return_type(cur) == DATA_ENTRY){
			if(new_file_ent == OFF){
				printf("There is an entries file\n");
				new_file_ent = ON;
				ent_file = fopen(ent_file_name, "w");
				if (ent_file == NULL) {
					printf("Error opening file!\n");
					free(ent_file_name);
					return;
		    	}
			}
			fprintf(ent_file,"%s  %04d\n", return_name(cur), return_value(cur));
		}
	}
	/*closing and freeing*/
	if(new_file_ext == OFF) printf("No .ext files for the %s file\n", ob_file_name);
	if(new_file_ent == OFF) printf("No .ent files for the %s file\n", ob_file_name);

	fclose(ob_file);
	if(ext_file) fclose(ext_file);
	if(ent_file) fclose(ent_file);
	free_list(temp);
	free(ob_file_name);
	free(ext_file_name);
	free(ent_file_name);
	
	
	
}


/**
 * @brief Counts the number of elements in a linked list.
 *
 * This function traverses the linked list and counts the number of elements.
 *
 * @param head The head of the linked list.
 * @return The number of elements in the linked list.
 */
int count_list(list* head){
	list* cur = head;
	int sum = 0;
	
	while(cur) {
		sum++;
		cur = return_next(cur);
	}
	return sum;
}	

/**
 * @brief Returns the type of a symbol in the linked list.
 *
 * This function searches for a symbol in the linked list and returns its type.
 *
 * @param name The name of the symbol to search for.
 * @param linked_list The linked list to search in.
 * @return The type of the symbol if found, otherwise -1.
 */
int return_type_symbol(char* name, list *linked_list){
	list *cur = linked_list;

	while(cur){
		if(strcmp(return_name(cur),name) == 0)
			return return_type(cur);
		cur = return_next(cur);
	}
	return -1;
}