#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "pre_assembler.h"
#include "assembler.h"

/*	while error checking, make sure entry and extern is good
	make function edit_or_add_to_list
	if its direct addresing it uses only 12 bits
*/
	
int main(int argc, char *argv[]) {
	int i;
	char* filename = NULL;
	flag error = OFF;
	flag allocation_error = OFF;
	list *command_list = create_new_list(&allocation_error);
	if(allocation_error == ON) allocation_error_handle("");		
    	
	/*Check if was porivided any file*/
	if (argc < 2) {
        	printf("Error: No input files provided\n");
        	return 1;
    	}
	
    	/* Process each file */
    	for (i = 1; i < argc; i++) {
            filename = add_postfix(argv[i], ".as", ADD, &allocation_error);
			if(allocation_error == ON) allocation_error_handle("l", command_list);	
            error = pre_assembler(filename, command_list);
            free(filename);/* Free allocated memory for filename after use */
			if(error == ON) {continue;} /*continue to the next file*/
			filename = add_postfix(argv[i], ".am", ADD, &allocation_error);
			if(allocation_error == ON) allocation_error_handle("l", command_list);	
			error = first_assembler(filename, command_list);
			free(filename);
			if(error == ON) continue;

			/*error = second_assembler()*/
			
    	}
	/*	for(temp = command_list; temp != NULL; temp = temp->next){
			printf("the name is: %s\n", temp->name);
		}*/
		free_list(command_list);

    return 0;
}