/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:07:36 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/06 02:36:14 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	n_flag(char **arg, int i)
{
	return (arg[i] && arg[i][0] == '-' && arg[i][1] == 'n' &&
	arg[i][skip_char(arg[i] + 1, 'n') + 1] == '\0');
}

void	ft_echo(t_all *all, char **arg)
{
	int	i;
	int	n;
	int	f;

	i = 0;
	n = 0;
	f = 0;
	if (n_flag(arg, 0) && ++i)
		n = 1;
	while (arg[i])
	{
		if (!n_flag(arg, i) || (n_flag(arg, i) && f))
		{
			write(1, arg[i], ft_strlen(arg[i]));
			if (arg[i + 1] && arg[i + 1][0] != '\0')
				write(1, " ", 1);
			f = 1;
		}
		i++;
	}
	if (!n)
		write(1, "\n", 1);
	all->exits = 0;
}

t_list	*search_lst(t_list *list, t_env *var)
{
	t_list	*f;

	f = NULL;
	while (list)
	{
		if (ft_strcmp(((t_env *)list->content)->name, var->name) == 0)
		{
			f = list;
			break ;
		}	
		list = list->next;
	}
	return (f);
}

void	ft_export_wa(t_all *all, t_env *var)
{
	t_list	*list;
	t_list	*exist;

	list = all->l_env;
	exist = search_lst(list, var);
	if (exist && var->value)
		exist->content = var;
	else if (!exist)
		ft_lstadd_back(&all->l_env, ft_lstnew(var));
}
