/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
void free_null_terminated_array_of_arrays(char **array);
char *get_pwd_var(char **envp);
char *get_command_path(char *cmd,char *cwd,char **pathvar_entries);
char *get_path_var(char **envp);
void redirect_stdout_fileno_to_fd(int outfile_fd);
void redirect_stdin_fileno_to_fd(int infile_fd);
void redirect_stdin_fileno_to_pipe_read_end(int pipe_read_end_fd);
void redirect_stdout_fileno_to_pipe_write_end(int pipe_write_end_fd);
