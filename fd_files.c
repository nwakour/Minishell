/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:06:22 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/16 14:21:54 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	s_tern(int cond, int iftrue, int iffalse)
{
	if (cond)
		return (iftrue);
	else
		return (iffalse);
}

void	get_files(int nextin, int nextout, t_cmd *cmd)
{
	cmd->infd = nextin;
	cmd->fd = nextout;
	if (cmd->in)
	{
		cmd->infd = open(cmd->in, O_RDONLY);
		free(cmd->in);
	}
	if (cmd->out)
	{
		if (cmd->append)
			cmd->fd = open(cmd->out, O_WRONLY | O_CREAT
					| O_APPEND, S_IRUSR | S_IWUSR);
		else
			cmd->fd = open(cmd->out, O_WRONLY | O_CREAT
					| O_TRUNC, S_IRUSR | S_IWUSR);
		free(cmd->out);
	}
}

static void	fd_update(t_cmd *cmd, int fd, char *name, int t)
{
	if (fd == -1)
		return ;
	if (t)
	{
		if (cmd->out)
			free(cmd->out);
		cmd->out = ft_strdup(name);
		cmd->append = s_tern((t == 2), 1, 0);
	}
	else
	{
		if (cmd->in)
			free(cmd->in);
		cmd->in = ft_strdup(name);
	}
}

static int	fd_treat_files(t_cmd *cmd, int i)
{
	int	fd;

	if (cmd->f_name[i][0] == '?')
	{
		fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT
				| O_APPEND, S_IRUSR | S_IWUSR);
		fd_update(cmd, fd, (cmd->f_name[i] + 1), 2);
	}
	else if (cmd->f_name[i][0] == '>')
	{
		fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT
				| O_TRUNC, S_IRUSR | S_IWUSR);
		fd_update(cmd, fd, (cmd->f_name[i] + 1), 1);
	}
	else
	{
		fd = open((cmd->f_name[i] + 1), O_RDONLY);
		fd_update(cmd, fd, (cmd->f_name[i] + 1), 0);
	}
	return (fd);
}

void	fd_files(t_all *all, t_cmd *cmd)
{
	int	i;
	int	fd;

	cmd->exec = 1;
	cmd->in = NULL;
	cmd->out = NULL;
	if (cmd->f_name && cmd->f_name[0])
	{
		i = -1;
		while (cmd->f_name[++i])
		{
			fd = fd_treat_files(cmd, i);
			if (fd == -1)
			{
				s_perror(all, cmd->f_name[i] + 1, 1);
				cmd->exec = 0;
				break ;
			}
			close(fd);
		}
	}
}
