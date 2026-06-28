#include "definitions.h"
#include "data_struct.h"
#include "helper.h"
#include "pre_assembler.h"
#include "assembler.h"
	
int main(int argc, char *argv[]) {
	int i;
	char* filename = NULL;
	flag error = OFF;
	flag allocation_error = OFF;
	list *command_list;	
    	
	/*Check if was porivided any file*/
	if (argc < 2) {
        	printf("Error: No input files provided\n");
        	return 1;
    	}
	
    	/* Process each file */
    	for (i = 1; i < argc; i++) {
			command_list = create_new_list(&allocation_error);
			if(allocation_error == ON) allocation_error_handle("");	/*exit if allocation error accured*/
            filename = add_postfix(argv[i], ".as", ADD, &allocation_error); /*add the.as to the filename*/
			if(allocation_error == ON) allocation_error_handle("l", command_list);	/*exit if allocation error accured*/
            error = pre_assembler(filename, command_list); /*the macro level*/
            free(filename);/* Free allocated memory for filename after use */
			if(error == ON){ /*continue to the next file*/
				free_list(command_list);
				continue;
			} 
			filename = add_postfix(argv[i], ".am", ADD, &allocation_error);
			if(allocation_error == ON) allocation_error_handle("l", command_list);	
			error = first_assembler(filename, command_list);
			if(error == ON) printf("Couldn't create the %s object file\n", filename);
			else printf("Object file for %s created sucssesfuli\n", argv[i]);
			free(filename);
			free_list(command_list);
    	}
		

    return 0;
}