/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:06:22 by nwakour           #+#    #+#             */
/*   Updated: 2021/07/10 18:00:49 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fd_files(t_all *all, t_cmd *cmd)
{
	int i;

	// to check later if there's no problem n cmd should be executed
	cmd->fd = 1;
	cmd->exec = 1;
	cmd->infd = all->nextin;
	if (cmd->f_name[0])
	{
		i = -1;
		while (cmd->f_name[++i])
		{
			if (cmd->f_name[i][0] == '?')
				cmd->fd = open((cmd->f_name[i] + 1), O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
			else if (cmd->f_name[i][0] == '>')
				cmd->fd = open((cmd->f_name[i] + 1), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
			else if (cmd->f_name[i][0] == '<')
			{
				cmd->infd = open((cmd->f_name[i] + 1), O_RDONLY);
			}
			else if (cmd->f_name[i][0] == '@')
				;//
			if (cmd->fd == -1)
			{
				s_perror(all, cmd->f_name[i], 1);
				all->error = 1; // all->error can be used to save exit status if there's no parsing error
				cmd->exec = 0;
			}
			// if (cmd->f_name[i + 1])
			// 	close(cmd->fd);
		}
	}
	else
		cmd->fd = 1;
}
