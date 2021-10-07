/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 14:53:16 by hmahjour          #+#    #+#             */
/*   Updated: 2021/10/06 04:34:50 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	s_perror(t_all *all, char *name, int err)
{
	int	len;

	len = ft_strlen(strerror(errno));
	write(2, name, ft_strlen(name));
	write(2, ": ", 2);
	write(2, strerror(errno), len);
	write(2, "\n", 1);
	all->exits = err;
}

void	s_wait(t_all *all, t_cmd *cmd)
{
	int	status;

	waitpid(cmd->pid, &status, 0);
	g_child = 0;
	if (WIFEXITED(status))
		all->exits = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			all->exits = 130;
		else if (WTERMSIG(status) == SIGQUIT)
		{
			all->exits = 131;
			write(1, "Quit: 3\n", 8);
		}
	}
}

char	**s_env(t_all *all)
{
	char	**env;
	t_list	*tmp;
	int		i;
	int		sz;

	sz = ft_lstsize(all->l_env) + 1;
	env = (char **)malloc(sz * sizeof(char *));
	if (!env)
		return (NULL);
	tmp = all->l_env;
	i = -1;
	while (tmp)
	{
		env[++i] = s_join(((t_env *)tmp->content)->name,
				'=', ((t_env *)tmp->content)->value);
		tmp = tmp->next;
	}
	env[i + 1] = 0;
	return (env);
}

// void	s_last(t_all *all, t_cmd *cmd)
// {
// 	cmd->pid = fork();
// 	if (cmd->pid < 0)
// 		s_perror(all, "fork", 1);
// 	g_child = 1;
// 	if (cmd->pid == 0)
// 	{
// 		dup2(cmd->infd, 0);
// 		dup2(cmd->fd, 1);
// 		if (cmd->infd > 1)
// 			close(cmd->infd);
// 		if (cmd->fd > 1)
// 			close(cmd->fd);
// 		s_exec(all, cmd);
// 	}
// 	if (!all->pip)
// 		s_wait(all, cmd);
// 	g_child = 0;
// 	if (cmd->infd > 1)
// 		close(cmd->infd);
// 	if (cmd->fd > 1)
// 		close(cmd->fd);
// }

void	s_last(t_all *all, t_cmd *cmd)
{
	cmd->pid = fork();
	if (cmd->pid < 0)
		s_perror(all, "fork", 1);
	g_child = 1;
	if (cmd->pid == 0)
	{
		get_files(all->nextin, 1, cmd);
		dup2(cmd->infd, 0);
		dup2(cmd->fd, 1);
		if (cmd->infd > 0)
			close(cmd->infd);
		if (cmd->fd > 1)
			close(cmd->fd);
		s_exec(all, cmd);
	}
	if (!all->pip)
		s_wait(all, cmd);
	g_child = 0;
}

char	*s_join(char *name, char c, char *val)
{
	char	*res;
	int		i;
	int		j;

	res = (char *)malloc((ft_strlen(name) + ft_strlen(val) + 2) * sizeof(char));
	if (!name || !val || !res)
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
