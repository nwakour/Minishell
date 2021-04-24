/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:06:22 by nwakour           #+#    #+#             */
/*   Updated: 2021/04/22 14:01:58 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fd_files(t_cmd *cmd)
{
	int i;

	if (cmd->f_name[0])
	{
		i = -1;
		while (cmd->f_name[++i])
		{
			if (cmd->f_name[i][0] == '?')
				cmd->fd = open((cmd->f_name[i] + 1), O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
			else
				cmd->fd = open((cmd->f_name[i] + 1), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
			if (cmd->f_name[i + 1])
				close(cmd->fd);
		}
	}
	else
		cmd->fd = 1;
}
