/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:16:56 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:16:56 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "pipex.h"

void	redirect_fd_to_fd(int fd1, int fd2)
{
	dup2(fd2, fd1);
	close(fd2);
}

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
