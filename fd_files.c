/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:06:22 by nwakour           #+#    #+#             */
/*   Updated: 2021/09/19 15:40:10 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	fd_files(t_all *all, t_cmd *cmd)
{
	int i;
	int in;
	int out;

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
			if (cmd->f_name[i][0] == '?')
			{
				if (out > 1)
					close(out);
				cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
					out = cmd->fd;

			}
			else if (cmd->f_name[i][0] == '>')
			{
				if (out > 1)
					close(out);
				cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
				out = cmd->fd;
			}
			else if (cmd->f_name[i][0] == '<')
			{
				if (in > 1)
					close(in);
				cmd->infd = open((cmd->f_name[i] + 1), O_RDONLY);
				in = cmd->infd;
			}
			if (cmd->fd == -1 || cmd->infd == -1)
			{
				s_perror(all, cmd->f_name[i], 1);
				all->exits = 1; 
				cmd->exec = 0;
				break;
			}
		}
	}
}

void	s_dup(t_cmd *cmd)
{
	dup2(cmd->infd, 0);
	dup2(cmd->fd, 1);
	if (cmd->fd > 1)
		close(cmd->fd);
	if (cmd->infd > 1)
		close(cmd->infd);
}