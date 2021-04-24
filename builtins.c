/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:22:50 by abenouda          #+#    #+#             */
/*   Updated: 2021/04/24 16:05:24 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **arg, int fd)
{
	int i;
	int n;
	int f;

	i = 0;
	f = 0;
	n = 0;
	while (arg[i])
	{
		if (f == 0 && arg[i][0] == '-' && arg[i][1] == 'n')
		{
			if (arg[i][skip_char(arg[i] + 1, 'n') + 1] == '\0')
				n = 1;
			else
				f = 1;
		}
		else
				f = 1;
		if (f == 1)
			ft_putstr_fd(arg[i], fd);
		i++;
		if (f == 1 && arg[i] && arg[i][0] != '\0')
			ft_putstr_fd(" ", fd);
	}
	if (!n)
		ft_putstr_fd("\n", fd);
}

int		ft_cd(t_all *all, char* path, int args)
{
	int r;
	t_env *env;

	env = (t_env*)malloc(sizeof(t_env));
	env->name = "OLD_PWD";
	env->value = getcwd(NULL, 0);
	ft_export_wa(all, env);
	if (args == 1 && !path)
		return 0;
	if (args > 1)
	{
		printf("too many arguments\n");
		return 0;
	}
	if (path && path[0] == '~')
	{
		r = chdir(getenv("HOME"));
		if (path [1] && path[1] == '/' && path[2])
			r = chdir(path + 2);
	}
	else if (!path)
		r = chdir(getenv("HOME"));
	else
		r = chdir(path);
	if (r != 0)
		printf("cd: no such file or directory: %s\n", path);
	else
	{
		env = (t_env*)malloc(sizeof(t_env));
        env->name = "PWD";
        env->value = getcwd(NULL, 0);
    	ft_export_wa(all, env);
	}
	return (r);
}

void	ft_pwd(int fd)
{
	char *s;

	s = getcwd(NULL, 0);
	ft_putstr_fd(s, fd);
	ft_putstr_fd("\n", fd);
	free(s);
}

t_list	*search_lst(t_list *list, t_env *var)
{
	t_list *f;

	f = NULL;
	while (list)
	{
		if (ft_strcmp(((t_env*)list->content)->name, var->name) == 0)
		{
			f = list;
			break;
		}	
		list = list->next;
	}
	return (f);
}

void	ft_export_wa(t_all *all, t_env *var)
{
	t_list	*list;
	t_list 	*exist;
	list = all->l_env;
	exist = search_lst(list, var);
	if (exist && var->value)
		exist->content = var;
	else if(!exist)
		ft_lstadd_back(&all->l_env, ft_lstnew(var));
}

void	print_export(t_list *list, int fd)
{
	t_list *l;
	
	l = list;
	while (l)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(((t_env*)l->content)->name, fd);
		if (((t_env*)l->content)->value)
		{
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd(((t_env*)l->content)->value, fd);
			ft_putstr_fd("\"", fd);
		}
		ft_putstr_fd("\n", fd);	
		l = l->next;
	}
}

void	print_env(t_list *list, int fd)
{
	t_list *l;
	
	l = list;
	while (l)
	{
		if (((t_env*)l->content)->value)
		{
			ft_putstr_fd(((t_env*)l->content)->name, fd);
			ft_putstr_fd("=", fd);
			ft_putstr_fd(((t_env*)l->content)->value, fd);
			ft_putstr_fd("\n", fd);	
		}
		l = l->next;
	}
}

void	alpha_sort(t_list *list)
{
	t_list *b;
	void *tmp;
	
	b = NULL;
	while (list != NULL)
	{
		b = list->next;
		while (b != NULL)
		{
			if(ft_strcmp(((t_env*)list->content)->name, ((t_env*)b->content)->name) > 0)
			{
				tmp = list->content;
				list->content = b->content;
				b->content = tmp;
			}
			b = b->next;
		}
		list = list->next;
	}
}

void	ft_export_na(t_all *all, int fd)
{
	t_list *list;
	
	list = all->l_env;
	alpha_sort(list);
	print_export(list, fd);
}

void	ft_env(t_all *all, int fd)
{
	t_list *list;
	
	list = all->l_env;
	print_env(list, fd);
}

t_list	*search_lst_unset(t_list *list, char *var, t_list **before)
{
	t_list *f;
	int i;

	f = NULL;
	i = 0;
	while (list)
	{
		if (ft_strcmp(((t_env*)list->content)->name, var) == 0)
		{
			f = list;
			break;
		}
		if (i)
			*before = list;
		i = 1;
		list = list->next;
	}
	return (f);
}

void	ft_unset(t_all *all, char *var)
{
	t_list	*list;
	t_list	*f;
	t_list 	*before;

	before = NULL;
	list = all->l_env;
	f = search_lst_unset(list, var, &before);
	if (f)
	{
		if (before)
			before->next = f->next;
		else
		{
			list = list->next;
			all->l_env = list;
		}
		free(f->content);
		free(f);
	}
}

void	ft_exit(void)
{
	// free(line);
	// system("leaks minishell");
	exit(0);
}