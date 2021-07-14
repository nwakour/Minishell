/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 14:53:16 by hmahjour          #+#    #+#             */
/*   Updated: 2021/07/11 19:27:42 by hmahjour         ###   ########.fr       */
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
	write(2, "\n", 1);
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
	int	i;
	int	sz;

	sz = ft_lstsize(all->l_env) + 1;
	env = (char **)malloc(sz * sizeof(char *));
	if (!env)
		return (NULL);
	tmp = all->l_env;
	i = -1;
	while (tmp)
	{
		env[++i] = s_join(((t_env*)tmp->content)->name, '=',((t_env*)tmp->content)->value);
		tmp = tmp->next;	
	}
	env[i] = 0;
	return (env);
}

char	**s_args(t_cmd *cmd)
{
	char **tmp;
	int i;

	i = 0;
	tmp = (char **)malloc((cmd->args + 2) * sizeof(char*));
	tmp[i] = cmd->cmd;
	i++;
	while (*cmd->arg)
	{
		tmp[i] = *cmd->arg;
		cmd->arg++;
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}

void	displayenv(t_all *all)
{
	char **tmp = s_env(all);
	int i = 0;
	
	while (tmp[i])
	{
		write(2, tmp[i], ft_strlen(tmp[i]));
		write(2, "\n", 1);
		i++;
	}
}

void	s_found(t_all *all, struct stat *st, char *file, t_cmd *cmd)
{
	char **args = s_args(cmd);
	char **tmp = args;
	
	if (!lstat(file, st))
	{
		// write(2, "|", 1);
		// write(2, file, ft_strlen(file));
		// write(2, "|", 1);
		// while (*args)
		// {
		// 	write(2, "|", 1);
		// 	write(2, *(args), ft_strlen(*(args)));
		// 	write(2, "|", 1);
		// 	args++;
		// }
		//displayenv(all);
		if (execve(file, tmp, s_env(all)) == -1)
		{
			s_perror(all, cmd->cmd, 126);
			exit(126);
		}
	}
	else
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
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
		if (execve(cmd->cmd, s_args(cmd), s_env(all)) == -1)
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
		s_found(all, &st, file, cmd);
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
		if (cmd->fd != 1)
			dup2(cmd->fd, 1);
		else
			dup2(fd[1], 1);
		if (cmd->infd > 1)
			close(cmd->infd);
		if (cmd->fd > 1)
			close(cmd->fd);
		if (fd[0])
			close(fd[0]);
		if (fd[1] != 1)
			close(fd[1]);
		s_exec(all, cmd);
	}
	s_wait(all, cmd);
	close(fd[1]);
	if (cmd->infd > 1)
		close(cmd->infd);
	if (cmd->fd > 1)
		close(cmd->fd);
	// need to to put fd[0] as infd for next cmd
	all->nextin = fd[0];
}

void	s_last(t_all *all, t_cmd *cmd)
{
	//printf("%s %d %d\n", cmd->cmd, cmd->infd, cmd->fd);
	cmd->pid = fork();
	if (cmd->pid < 0)
		s_perror(all, "fork", 1);
	if (cmd->pid == 0)
	{
		dup2(cmd->infd, 0);
		dup2(cmd->fd, 1);
		if (cmd->infd > 1)
			close(cmd->infd);
		if (cmd->fd > 1)
			close(cmd->fd);
		s_exec(all, cmd);
	}
	s_wait(all, cmd);
	if (cmd->infd > 1)
			close(cmd->infd);
	if (cmd->fd > 1)
			close(cmd->fd);
}
