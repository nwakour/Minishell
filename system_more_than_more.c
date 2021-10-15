/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_more_than_more.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 04:19:11 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/15 10:04:28 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

char	**s_args(t_cmd *cmd)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = (char **)malloc((cmd->args + 2) * sizeof(char *));
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
