/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 14:53:16 by hmahjour          #+#    #+#             */
/*   Updated: 2021/07/08 18:06:07 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	s_perror(t_all *all, char *name, int err)
{
	int len;

	len = ft_strlen(strerror(errno));
	write(2, name, ft_strlen(name));
	write(2, ": ", 2);
	write(2, strerror(errno), len);
	all->error = err;
}

int	s_wait(t_all *all, t_cmd *cmd)
{
	int	status;

	waitpid(cmd->pid, &status, 0);
	if (WEXITED)
		all->error = WEXITSTATUS(status);
}

void	s_exec(t_all *all)
{
	//TODO: get env in char ** format, get path
}

void	s_cmd(t_all *all, t_cmd *cmd)
{
	int	fd[2];

	if (pipe(fd) < 0)
		s_perror(all, "pipe", 1);
	cmd->pid = fork();
	if (cmd->pid < 0)
		s_perror(all, "fork", 1);
	if (cmd->pid == 0)
	{
		dup2(cmd->infd, 0);
		dup2(fd[1], 1);
		if (cmd->infd)
			close(cmd->infd);
		close(fd[0]);
		close(fd[1]);
		// EXEC	
	}
	s_wait(all, cmd);
	close(fd[1]);
	if (cmd->infd)
			close(cmd->infd);
	// need to to put fd[0] as infd for next cmd
	all->nextin = fd[0];
}

void	s_last(t_all *all, t_cmd *cmd)
{
	cmd->pid = fork();
	if (cmd->pid < 0)
		s_perror(all, "fork", 1);
	if (cmd->pid == 0)
	{
		dup2(cmd->infd, 0);
		dup2(cmd->fd, 1);
		if (cmd->infd)
			close(cmd->infd);
		if (cmd->fd != 1)
			close(cmd->out)
		close(fd[0]);
		close(fd[1]);
		// EXEC	
	}
	s_wait(all, cmd);
	close(fd[1]);
	if (cmd->infd)
			close(cmd->infd);
	if (cmd->fd != 1)
			close(cmd->out)
	close(fd[0]);
	close(fd[1]);
}