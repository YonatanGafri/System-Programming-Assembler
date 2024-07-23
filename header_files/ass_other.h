

int find_register(char *word);
int check_label(char *word);
int find_command(char *word);
flag command_options(int command_vector[]);
int find_addresing(char *word);
flag check_lable_define(char *line, list *command_list, int line_num, char *file_name, flag *allocation_error);
flag check_line_length(char *line, int line_num, char *file_name);