# Target executable
assembler: object_files_dir object_files/main.o object_files/pre_assembler.o object_files/assembler.o object_files/helper_functions.o object_files/check_text.o object_files/ass_other.o object_files/data_struct.o
	gcc -ansi -Wall -g -pedantic -o assembler object_files/main.o object_files/pre_assembler.o object_files/assembler.o object_files/helper_functions.o object_files/check_text.o object_files/ass_other.o object_files/data_struct.o

# Create the object_files directory if it doesn't exist
object_files_dir:
	mkdir -p object_files

# Compile main.c
object_files/main.o: source_code/main.c
	gcc -c -ansi -Wall -pedantic -g -Iheader_files source_code/main.c -o object_files/main.o

# Compile pre_assembler.c
object_files/pre_assembler.o: source_code/pre_assembler.c
	gcc -c -ansi -Wall -pedantic -g -Iheader_files source_code/pre_assembler.c -o object_files/pre_assembler.o

# Compile assembler.c
object_files/assembler.o: source_code/assembler.c
	gcc -c -ansi -Wall -pedantic -g -Iheader_files source_code/assembler.c -o object_files/assembler.o

# Compile helper_functions.c
object_files/helper_functions.o: source_code/helper_functions.c
	gcc -c -ansi -Wall -pedantic -g -Iheader_files source_code/helper_functions.c -o object_files/helper_functions.o

# Compile check_text.c
object_files/check_text.o: source_code/check_text.c
	gcc -c -ansi -Wall -pedantic -g -Iheader_files source_code/check_text.c -o object_files/check_text.o

# Compile ass_other.c
object_files/ass_other.o: source_code/ass_other.c
	gcc -c -ansi -Wall -pedantic -g -Iheader_files source_code/ass_other.c -o object_files/ass_other.o

# Compile data_struct.c
object_files/data_struct.o: source_code/data_struct.c
	gcc -c -ansi -Wall -pedantic -g -Iheader_files source_code/data_struct.c -o object_files/data_struct.o

# Clean up object files and the executable
clean:
	rm -f object_files/main.o object_files/pre_assembler.o object_files/assembler.o object_files/helper_functions.o object_files/check_text.o object_files/ass_other.o object_files/data_struct.o assembler
	rm -rf object_files

