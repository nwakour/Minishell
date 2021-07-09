/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 14:53:16 by hmahjour          #+#    #+#             */
/*   Updated: 2021/07/09 17:57:36 by nwakour          ###   ########.fr       */
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

void	s_wait(t_all *all, t_cmd *cmd)
{
	int	status;

	waitpid(cmd->pid, &status, 0);
	if (WEXITED)
		all->error = WEXITSTATUS(status);
}

char	*s_join(char *name, char c, char *val)
{
	char	*res;
	int	i;
	int j;
	
	res = (char *)malloc((ft_strlen(name) + ft_strlen(val) + 2) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (name[i] != '\0')
	{
		res[i] = name[i];
		i++;
	}
	res[i] = c;
	i++;
	while (val[j] != '\0')
	{
		res[i + j] = val[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}

char	**s_env(t_all *all)
{
	char	**env;
	t_list	*tmp;
	int	sz;

	sz = ft_lstsize(all->l_env);
	env = (char **)malloc(sz * sizeof(char *));
	if (!env)
		return (NULL);
	tmp = all->l_env;
	while (tmp)
	{
		*env = s_join(((t_env*)tmp->content)->name, '=',((t_env*)tmp->content)->value);
		env++;
		tmp = tmp->next;	
	}
	return (env);
}

void	s_found(t_all *all, struct stat *st, char *file)
{
	if (!lstat(file, st) && S_ISREG(st->st_mode))
	{
		if (execve(file, all->cmd->arg, s_env(all)) == -1)
		{
			s_perror(all, all->cmd->cmd, 126);
			exit(126);
		}
	}
	else
	{
		write(2, all->cmd->cmd, ft_strlen(all->cmd->cmd));
		write(2, ": command not found\n", 20);
		exit(127);
	}
}

void	s_exec(t_all *all, t_cmd *cmd)
{
	char	**paths;
	char	*file;
	struct stat st;

	paths = s_paths(all);
	if (cmd->cmd && cmd->cmd[0] == '/')
	{
		if (execve(cmd->cmd, cmd->arg, s_env(all)) == -1)
		{
			s_perror(all, cmd->cmd, 126);
			exit(126);
		}
	}
	else
	{
		file = s_join(*paths, '/', cmd->cmd);
		while (*paths && lstat(file, &st))
		{
			paths++;
			if (*paths)
				file = s_join(*paths, '/', cmd->cmd);
		}
		s_found(all, &st, file);
	}
}

void	s_cmd(t_all *all, t_cmd *cmd)
{
	int	fd[2]; // needs to be moved to big loop

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
		s_exec(all, cmd);
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
			close(cmd->fd);
		s_exec(all, cmd);
	}
	s_wait(all, cmd);
	if (cmd->infd)
			close(cmd->infd);
	if (cmd->fd != 1)
			close(cmd->fd);
	s_wait(all, cmd);
}