/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_job.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 17:52:28 by tpouget           #+#    #+#             */
/*   Updated: 2022/02/27 17:52:28 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_JOB_H
# define T_JOB_H

typedef struct s_job {
	char	**pathvar_entries;
	char	**envp;
	char	**cmds;
	int		nb_cmds;
	int		curr_cmd_idx;
}	t_job;

#endif /* T_JOB_H */
