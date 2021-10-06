/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:06:22 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/06 02:59:09 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fd_more_files(t_cmd *cmd, int i, int out, int in)
{
	if (cmd->f_name[i][0] == '?')
	{
		if (out > 1)
			close(out);
		cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT
				| O_APPEND, S_IRUSR | S_IWUSR);
		out = cmd->fd;
	}
	else if (cmd->f_name[i][0] == '>')
	{
		if (out > 1)
			close(out);
		cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT
				| O_TRUNC, S_IRUSR | S_IWUSR);
		out = cmd->fd;
	}
	else if (cmd->f_name[i][0] == '<')
	{
		if (in > 1)
			close(in);
		cmd->infd = open((cmd->f_name[i] + 1), O_RDONLY);
		in = cmd->infd;
	}
}

void	fd_files(t_all *all, t_cmd *cmd)
{
	int	i;
	int	in;
	int	out;

	in = -1;
	out = -1;
	cmd->exec = 1;
	cmd->fd = 1;
	cmd->infd = all->nextin;
	if (cmd->f_name && cmd->f_name[0])
	{
		i = -1;
		while (cmd->f_name[++i])
		{
			fd_more_files(cmd, i, out, in);
			if (cmd->fd == -1 || cmd->infd == -1)
			{
				s_perror(all, cmd->f_name[i], 1);
				all->exits = 1;
				cmd->exec = 0;
				break ;
			}
		}
	}
}
