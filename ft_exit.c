/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:43:57 by nwakour           #+#    #+#             */
/*   Updated: 2021/11/04 16:33:33 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	s_isnum(char *str)
{
	int	ret;
	int	i;

	ret = 1;
	i = 0;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			ret = 0;
			break ;
		}
		i++;
	}
	return (ret);
}

static void	ft_exit_more(t_all *all, t_cmd *cmd)
{
	if (s_isnum(cmd->arg[0]) && cmd->args == 1)
	{
		all->exits = ft_atoi(cmd->arg[0]);
		if (!all->pip)
		{
			write(2, "exit\n", 5);
			exit(all->exits);
		}
	}
	if (cmd->args > 1)
	{
		write(2, "exit: ", 6);
		write(2, "too many arguments\n", 19);
		all->exits = 1;
	}
}

void	ft_exit(t_all *all, t_cmd *cmd)
{
	if (cmd->args == 0)
	{
		if (!all->pip)
			exit(all->exits);
		all->exits = 0;
	}
	if (cmd->args >= 1)
	{
		if (!s_isnum(cmd->arg[0])
			|| ft_strcmp(cmd->arg[0], "9223372036854775807") > 0
			|| ft_strlen(cmd->arg[0]) > 19)
		{
			if (!all->pip)
				write(2, "exit\nexit: ", 12);
			else
				write(2, "exit: ", 6);
			write(2, cmd->arg[0], ft_strlen(cmd->arg[0]));
			write(2, ": numeric argument required\n", 29);
			all->exits = 255;
			if (!all->pip)
				exit(255);
		}
		ft_exit_more(all, cmd);
	}
}

t_all	*get_exit__ptr(t_all *exit)
{
	static t_all	*_exit = NULL;

	if (exit)
	{
		_exit = exit;
		return (_exit);
	}
	return (_exit);
}
