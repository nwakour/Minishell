/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:06:22 by nwakour           #+#    #+#             */
/*   Updated: 2021/07/11 14:28:11 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	fd_files(t_all *all, t_cmd *cmd)
{
	int i;
	int prev[3];

	prev[0] = -1;
	prev[1] = -1;
	prev[2] = -1;
	cmd->exec = 1;
	cmd->fd = 1;
	cmd->infd = all->nextin;
	if (cmd->f_name[0])
	{
		i = -1;
		while (cmd->f_name[++i])
		{
			if (cmd->f_name[i][0] == '?')
			{
				if (prev[0] > 1)
					close(prev[0]);
				cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
					prev[0] = cmd->fd;

			}
			else if (cmd->f_name[i][0] == '>')
			{
				if (prev[1] > 1)
					close(prev[1]);
				cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
				prev[1] = cmd->fd;
			}
			else if (cmd->f_name[i][0] == '<')
			{
				if (prev[2] > 1)
					close(prev[2]);
				cmd->infd = open((cmd->f_name[i] + 1), O_RDONLY);
				prev[2] = cmd->infd;
			}
			if (cmd->fd == -1 || cmd->infd == -1)
			{
				s_perror(all, cmd->f_name[i], 1);
				all->exits = 1; 
				cmd->exec = 0;
				//break;
			}
		}
	}
}



// void	fd_files(t_all *all, t_cmd *cmd)
// {
// 	int i;
// 	int tmpin;
// 	int tmpout;
// 	// to check later if there's no problem n cmd should be executed
// 	tmpin = -1;
// 	tmpout = -1;
// 	cmd->fd = 1;
// 	cmd->exec = 1;
// 	cmd->infd = all->nextin;
// 	if (cmd->f_name[0])
// 	{
// 		i = -1;
// 		while (cmd->f_name[++i])
// 		{
// 			if (cmd->f_name[i][0] == '?')
// 			{
// 				cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
				
// 				// if (cmd->out)
// 				// 	free(cmd->out);
// 				// cmd->out = ft_strdup(cmd->f_name[i] + 1);
// 				// cmd->append = 1;
// 			}
// 			else if (cmd->f_name[i][0] == '>')
// 			{
// 				cmd->fd = open((cmd->f_name[i] + 1), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
// 				// if (cmd->out)
// 				// 	free(cmd->out);
// 				// cmd->out = ft_strdup(cmd->f_name[i] + 1);
// 				// cmd->append = 0;
// 			}
// 			else if (cmd->f_name[i][0] == '<')
// 			{
// 				cmd->infd = open((cmd->f_name[i] + 1), O_RDONLY);
// 				// if (cmd->in)
// 				// 	free(cmd->in);
// 				// cmd->in = ft_strdup(cmd->f_name[i] + 1);
// 			}
// 			if (cmd->fd == -1 || cmd->infd == -1)
// 			{
// 				s_perror(all, cmd->f_name[i], 1);
// 				all->exits = 1; 
// 				cmd->exec = 0;
// 			}
// 			if (cmd->fd > 1)
// 				close(cmd->fd);
// 			if (cmd->infd > 1)
// 				close(cmd->infd);
// 			// if (cmd->f_name[i + 1])
// 			// 	close(cmd->fd);
// 		}
// 		s_cmd_files(cmd);
// 	}
// 	else
// 		cmd->fd = 1;
// }

// void	s_cmd_files(t_cmd *cmd)
// {
// 	if (cmd->fd != 1)
// 	{
// 	if (cmd->append)
// 	{
// 		cmd->fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
// 	}
// 	else
// 	{
// 		cmd->fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
// 	}
// 	}
// 	if (cmd->infd)
// 		cmd->infd = open(cmd->in, O_RDONLY);
// }
