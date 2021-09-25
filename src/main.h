/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
void redirect_fd_to_fd(int fd1,int fd2);
void free_null_terminated_array_of_arrays(char **array);
char *get_pwd_var(char **envp);
char *get_command_path(char *cmd,char *cwd,char **pathvar_entries);
char *get_path_var(char **envp);
void execute_pipeline(char *cmd_str,int read_from,int write_to,char **env);
