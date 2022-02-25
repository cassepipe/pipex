/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_usage_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 21:16:56 by tpouget           #+#    #+#             */
/*   Updated: 2021/10/02 21:16:56 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

void	print_usage_exit(void)
{
	static const char	usage[] = {
		"Usage : "
		"./pipex infile \"cmd_1 [--option]...\" \"cmd_2 [--option]...\" outfile\n"
	};

	write(STDOUT_FILENO, usage, sizeof(usage));
	exit(EXIT_FAILURE);
}
