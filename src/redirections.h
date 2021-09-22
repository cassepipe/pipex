/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
void redirect_stdout_fileno_to_fd(int outfile_fd);
void redirect_stdin_fileno_to_fd(int infile_fd);
void redirect_stdin_fileno_to_pipe_read_end(int pipe_read_end_fd);
void redirect_stdout_fileno_to_pipe_write_end(int pipe_write_end_fd);
void redirect_fd_to_fd(int fd1,int fd2);
