/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:28:44 by tpouget           #+#    #+#             */
/*   Updated: 2022/01/06 22:18:32 by cassepipe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>

void	find_exec(char *cmd_str, char **env, char **pathvar_entries);
void	free_null_terminated_array_of_arrays(char **array);
char	**ft_split(char const *s, char c);
char	*get_command_path(char **cmdv, char *cwd, char **pathvar_entries);
char	*get_path_var(char **envp);
char	*get_pwd_var(char **envp);
void	print_usage_exit(void);
void	redirect_fd_to_fd(int fd1, int fd2);
void	redirect_stdout_fileno_to_pipe_write_end(int pipe_write_end_fd);
void	redirect_stdin_fileno_to_pipe_read_end(int pipe_read_end_fd);
void	redirect_stdin_fileno_to_fd(int infile_fd);
void	redirect_stdout_fileno_to_fd(int outfile_fd);
char	*retrieve_file_path_in_cwd(char *cwd, char *cmd);
char	*retrieve_command_in_pathvar(char *cmd, char **pathvar_entries);
char	*ft_empty_string(void);
char	*ft_strjoin3(char const *s1, char const *s2, char const *s3);
char	*ft_strchr(const char *s, int c);
int		ft_strncmp(const char *l, const char *r, size_t n);
char	*ft_strndup(const char *s, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
char	*ft_strjoin(char const *s1, char const *s2, size_t *len);
void	ft_puts_stderr(char *str);
void	ft_perror(const char *prg_name, const char *about, const char *err_msg);
int		ft_open(char *filename, int flags, mode_t mode);
void	pipe_or_die(int *pipe_fds);
void	file_is_ok_or_die(char **cmdv, char **pathvar_entries);

#endif
