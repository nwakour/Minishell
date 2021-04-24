/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:02:59 by nwakour           #+#    #+#             */
/*   Updated: 2021/04/24 16:22:45 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parce_env(t_all *all, char **env)
{
	int 	i;
	int		j;

	i = -1;
	while (env[++i])
	{
		j = 0;
		// if (env[i][0] == 'A' && env[i][1] == 'A')
		// 	printf("%s\n", env[i]);
		while (env[i][j] && env[i][j] != '=')
			j++;
		all->env = NULL;
		if (!(ft_struct_list(&all->l_env, (void**)&all->env, sizeof(t_env))))
			return ;
		all->env->name = ft_strndup(env[i], j);
		all->env->value = ft_strdup(env[i] + j + 1);
		// else
		// {
		// 	all->env->name = ft_strdup(env[i]);
		// 	all->env->value = NULL;
		// }
	}
}

char	*check_env(t_list *list, char *str)
{
	t_list	*tmp;

	tmp = list;
	while (tmp)
	{
		if (!ft_strcmp(((t_env*)tmp->content)->name, str + 1) && ((t_env*)tmp->content)->value != NULL)
			return (ft_strdup(((t_env*)tmp->content)->value));
		tmp = tmp->next;
	}
	return (NULL);
}
