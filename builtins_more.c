/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:07:17 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/19 20:01:18 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_update_pwd(t_all *all, char *var)
{
	t_list	*tmp;

	tmp = all->l_env;
	while (tmp)
	{
		if (!(ft_strcmp(((t_env *)tmp->content)->name, var)))
		{
			free(((t_env *)tmp->content)->value);
			((t_env *)tmp->content)->value = getcwd(NULL, 0);
			break ;
		}
		tmp = tmp->next;
	}
}

static void	ft_cd_home(t_all *all)
{
	int	ret;

	ret = chdir(getenv("HOME"));
	if (ret != 0)
	{
		write(2, "cd: HOME not set\n", 18);
		all->exits = 1;
	}
}

static void	ft_follow_cd(t_all *all, int r, char *path)
{
	if (r != 0)
	{
		write(2, "cd: ", 4);
		write(2, path, ft_strlen(path));
		write(2, ": no such file or directory\n", 28);
		all->exits = 1;
	}
	else
		ft_update_pwd(all, "PWD");
}

void	ft_cd(t_all *all, char *path, int args)
{
	int	r;

	r = 0;
	ft_update_pwd(all, "OLDPWD");
	all->exits = 0;
	if (args == 1 && !path)
		return ;
	if (args > 1)
	{
		write(2, "cd: too many arguments\n", 23);
		all->exits = 1;
		return ;
	}
	if (!path)
		ft_cd_home(all);
	else if (path && path[0] == '~')
	{
		r = chdir(getenv("HOME"));
		if (path[1] && path[1] == '/' && path[2])
			r = chdir(path + 2);
	}	
	else
		r = chdir(path);
	ft_follow_cd(all, r, path);
}

void	ft_pwd(t_all *all)
{
	char	*s;

// check if NULL
	s = getcwd(NULL, 0);
	if (s)
	{
		write(1, s, ft_strlen(s));
		write(1, "\n", 1);
		free(s);
		all->exits = 0;
	}
	else
		s_perror(all, "pwd", 1);
}
