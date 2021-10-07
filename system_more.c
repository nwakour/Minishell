/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_more.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:27:49 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/06 04:30:56 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

static void	s_exec_more(t_all *all, t_cmd *cmd, char **paths, struct stat *st)
{
	char	*file;

	if (!paths)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	file = s_join(*paths, '/', cmd->cmd);
	while (*paths && lstat(file, st))
	{
		paths++;
		if (*paths)
			file = s_join(*paths, '/', cmd->cmd);
	}
	all->cmd = cmd;
	s_found(all, st, file, s_args(cmd));
}

void	s_exec(t_all *all, t_cmd *cmd)
{
	char		**paths;
	struct stat	st;

	paths = s_paths(all);
	if (cmd->cmd && cmd->cmd[0] == '/')
	{
		if (execve(cmd->cmd, s_args(cmd), all->envp) == -1)
		{
			s_perror(all, cmd->cmd, 126);
			exit(126);
		}
	}
	else
		s_exec_more(all, cmd, paths, &st);
}

static void	s_check_exec(t_all *all, t_cmd *cmd)
{
	if (cmd->valid == 1)
	{
		execute_cmd(all, cmd);
		exit(all->exits);
	}
	else if (cmd->valid == 2 && cmd->exec && all->pip)
		s_exec(all, cmd);
}

// static void	s_cmd_more(t_all *all, t_cmd *cmd, int *fd)
// {
// 	g_child = 1;
// 	if (cmd->pid == 0)
// 	{
// 		dup2(cmd->infd, 0);
// 		if (cmd->fd != 1)
// 			dup2(cmd->fd, 1);
// 		else
// 			dup2(fd[1], 1);
// 		if (cmd->infd > 1)
// 			close(cmd->infd);
// 		if (cmd->fd > 1)
// 			close(cmd->fd);
// 		if (fd[0] > 1)
// 			close(fd[0]);
// 		if (fd[1] > 1)
// 			close(fd[1]);
// 		s_check_exec(all, cmd);
// 	}
// }

static void	s_cmd_more(t_all *all, t_cmd *cmd, int *fd)
{
	g_child = 1;
	if (cmd->pid == 0)
	{
		get_files(all->nextin, fd[1], cmd);
		if (!cmd->exec)
			exit(all->exits);
		dup2(cmd->infd, 0);
		dup2(cmd->fd, 1);
		if (cmd->infd > 0)
			close(cmd->infd);
		close(cmd->fd);
		close(fd[0]);
		close(fd[1]);
		s_check_exec(all, cmd);
	}
}


void	s_cmd(t_all *all, t_cmd *cmd)
{
	int	fd[2];

	if (pipe(fd) < 0)
		s_perror(all, "pipe", 1);
	cmd->pid = fork();
	if (cmd->pid < 0)
		s_perror(all, "fork", 1);
	s_cmd_more(all, cmd, fd);
	g_child = 0;
	close(fd[1]);
	all->nextin = fd[0];
}
