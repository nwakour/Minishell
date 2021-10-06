/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtind_more_than_more.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:10:19 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/06 03:28:15 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_list *list)
{
	t_list	*l;

	l = list;
	while (l)
	{
		write(1, "declare -x ", 11);
		write(1, ((t_env *)l->content)->name,
			ft_strlen(((t_env *)l->content)->name));
		if (((t_env *)l->content)->value)
		{
			write(1, "=\"", 2);
			write(1, ((t_env *)l->content)->value,
				ft_strlen(((t_env *)l->content)->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		l = l->next;
	}
}

static void	print_env(t_list *list)
{
	t_list	*l;

	l = list;
	while (l)
	{
		if (((t_env *)l->content)->value)
		{
			write(1, ((t_env *)l->content)->name,
				ft_strlen(((t_env *)l->content)->name));
			write(1, "=", 1);
			write(1, ((t_env *)l->content)->value,
				ft_strlen(((t_env *)l->content)->value));
			write(1, "\n", 1);
		}
		l = l->next;
	}
}

void	alpha_sort(t_list *list)
{
	t_list	*b;
	void	*tmp;

	b = NULL;
	while (list != NULL)
	{
		b = list->next;
		while (b != NULL)
		{
			if (ft_strcmp(((t_env *)list->content)->name,
					((t_env *)b->content)->name) > 0)
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

void	ft_env(t_all *all)
{
	t_list	*list;

	list = all->l_env;
	print_env(list);
	if (all->envp)
		free(all->envp);
	all->envp = s_env(all);
	all->exits = 0;
}
