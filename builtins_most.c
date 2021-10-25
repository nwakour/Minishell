/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_most.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:11:34 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/25 20:24:08 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_export_na(t_all *all)
{
	t_list	*list;

	list = all->l_env;
	alpha_sort(list);
	print_export(list);
}

void	ft_export(t_all *all, t_cmd *cmd)
{
	int	i;

	if (cmd->arg[0])
	{
		i = -1;
		while (cmd->arg[++i])
			export_parse(all, cmd->arg[i]);
	}
	else
		ft_export_na(all);
	if (all->envp)
	{
		i = -1;
		while (all->envp[++i])
			free(all->envp[i]);
		free(all->envp);
	}
	all->envp = s_env(all);
	all->exits = 0;
}

static t_list	*search_lst_unset(t_list *list, char *var, t_list **before)
{
	t_list	*f;
	int		i;

	f = NULL;
	i = 0;
	while (list)
	{
		if (ft_strcmp(((t_env *)list->content)->name, var) == 0)
		{
			f = list;
			break ;
		}
		if (i)
			*before = list;
		i = 1;
		list = list->next;
	}
	return (f);
}

void	ft_unset_co(t_all *all, char *var)
{
	t_list	*list;
	t_list	*f;
	t_list	*before;

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
		free(((t_env *)f->content)->name);
		free(((t_env *)f->content)->value);
		free(f->content);
		free(f);
	}
}

void	ft_unset(t_all *all, t_cmd *cmd)
{
	int	i;

	if (cmd->arg[0])
	{
		i = -1;
		while (cmd->arg[++i])
			ft_unset_co(all, cmd->arg[i]);
	}
	if (all->envp)
	{
		i = -1;
		while (all->envp[++i])
			free(all->envp[i]);
		free(all->envp);
	}
	all->envp = s_env(all);
	all->exits = 0;
}
