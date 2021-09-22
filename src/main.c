#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "libft.h"
#include "main.h"

void	redirect_stdout_fileno_to_pipe_write_end(int pipe_write_end_fd)
{
		dup2(pipe_write_end_fd, STDOUT_FILENO);
		close(pipe_write_end_fd);
}

void	redirect_stdin_fileno_to_pipe_read_end(int pipe_read_end_fd)
{
		dup2(pipe_read_end_fd, STDIN_FILENO);
		close(pipe_read_end_fd);
}

void	redirect_stdin_fileno_to_fd(int infile_fd)
{
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
}

void	redirect_stdout_fileno_to_fd(int outfile_fd)
{
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
}


int	main(int ac, char **av, char **envp)
{
	int		pipefd[2];
	int		child_pid;
	int		infile_fd;
	int		outfile_fd;
	char	**cmd1;
	char	**cmd2;
	char	**pathvar_entries;

	if ((infile_fd = open(av[1], O_RDONLY)) < 0)
		perror("open ");
	if ((outfile_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		perror("open ");
	if (pipe(pipefd) == -1)
		perror("pipe ");

	pathvar_entries = ft_split(get_path_var(envp), ':');

	child_pid = fork();
	if (child_pid == -1)
		perror("fork ");
	else if (child_pid ==  0)
	{
		//The child is never going to use the read end of the pipe
		close(pipefd[0]);

		//grep reads from fd 0 so 0 must point to infile
		redirect_stdin_fileno_to_fd(infile_fd);

		//grep writes to fd 1 so 1 must point to the write end of the pipe
		redirect_stdout_fileno_to_pipe_write_end(pipefd[1]);

		//Retrieving the args
		cmd1 = ft_split(av[2], ' ');

		//Get the command path
		cmd1[0] = get_command_path(cmd1[0], get_pwd_var(envp), pathvar_entries);

		if (execve(cmd1[0], cmd1, envp) == -1)
		{
			perror("execve 1:");
		}
		free_null_terminated_array_of_arrays(cmd1);
		free_null_terminated_array_of_arrays(pathvar_entries);
	}
	else
	{
		//The parent is never goind to use the write end of the pipe
		close(pipefd[1]);

		//wc reads from 0 so 0 must point to the read end of the pipe
		redirect_stdin_fileno_to_pipe_read_end(pipefd[0]);

		//wc write to the 1 so 1 must point to the outfile
		redirect_stdout_fileno_to_fd(outfile_fd);

		//Retrieving args
		cmd2 = ft_split(av[3], ' ');

		//Get the command path
		cmd2[0] = get_command_path(cmd2[0], get_pwd_var(envp), pathvar_entries);

		if (execve(cmd2[0], cmd2, envp) == -1)
		{
			perror("execve 2:");
		}
		free_null_terminated_array_of_arrays(cmd2);
		free_null_terminated_array_of_arrays(pathvar_entries);

	}
	return (0);
}
