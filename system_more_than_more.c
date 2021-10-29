/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_more_than_more.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:44:48 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/29 15:44:49 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

char	**s_args(t_cmd *cmd)
{
	t_cmd	*tcmd;
	char	**tmp;
	int		i;

	i = 0;
	tcmd = cmd;
	tmp = (char **)malloc((tcmd->args + 2) * sizeof(char *));
	tmp[i] = tcmd->cmd;
	i++;
	while (*tcmd->arg)
	{
		tmp[i] = *tcmd->arg;
		tcmd->arg++;
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}

void	s_found(t_all *all, struct stat *st, char *file, char **args)
{
	char	**tmp;

	tmp = args;
	if (!lstat(file, st))
	{
		if (execve(file, tmp, all->envp) == -1)
		{
			s_perror(all, all->cmd->cmd, 126);
			exit(126);
		}
	}
	else if (!lstat(all->cmd->cmd, st))
	{
		if (execve(all->cmd->cmd, tmp, all->envp) == -1)
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

void	s_exit_error(char *cmd, char *msg, int excode)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, ft_strlen(msg));
	exit(excode);
}

void	s_exec_most(t_all *all, char **tmp, char *file, struct stat *st)
{
	if (!lstat(file, st))
	{
		if (execve(file, tmp, all->envp) == -1)
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

char	*update_path(t_all *all, int old)
{
	t_list	*tmp;
	char	*path;

	tmp = all->l_env;
	path = NULL;
	if (!old)
		path = getcwd(NULL, 0);
	else
	{
		while (tmp)
		{
			if (!(ft_strcmp(((t_env *)tmp->content)->name, "PWD")))
			{
				path = ft_strdup(((t_env *)tmp->content)->value);
				break ;
			}
			tmp = tmp->next;
		}
		if (!path)
			path = ft_strdup("");
	}
	return (path);
}
