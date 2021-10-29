/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:43:50 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/29 15:43:51 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parce_env(t_all *all, char **env)
{
	int	i;
	int	j;

	i = -1;
	while (env[++i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		all->env = NULL;
		if (!(ft_struct_list(&all->l_env, (void **)&all->env, sizeof(t_env))))
			return ;
		all->env->name = ft_strndup(env[i], j);
		all->env->value = ft_strdup(env[i] + j + 1);
	}
}

char	*check_env(t_list *list, char *str)
{
	t_list	*tmp;

	tmp = list;
	while (tmp)
	{
		if (!ft_strcmp(((t_env *)tmp->content)->name, str + 1)
			&& ((t_env *)tmp->content)->value != NULL)
			return (ft_strdup(((t_env *)tmp->content)->value));
		tmp = tmp->next;
	}
	return (NULL);
}

static void	more_export_parse(t_all *all, char*s, int i)
{
	t_env	*env;

	if (s[i] == '=')
	{
		env = (t_env *)malloc(sizeof(t_env));
		if (!env)
			return ;
		if (s[i])
		{
			env->name = ft_strndup(s, i);
			env->value = ft_strdup(s + i + 1);
		}
		else
		{
			env->name = ft_strdup(s);
			env->value = NULL;
		}
		ft_export_wa(all, env);
	}
}

int	export_parse(t_all *all, char *s)
{
	int		i;

	i = -1;
	if (s[0] && !ft_isalpha(s[0]) && s[0] != '_')
	{
		write(1, "export: `", 8);
		write(1, s, ft_strlen(s));
		write(1, "': not a valid identifier\n", 26);
		return (1);
	}
	while (s[++i] && s[i] != '=')
	{
		if (!ft_isalpha(s[i]) && !ft_isdigit(s[i]) && s[i] != '_')
		{
			write(1, "export: `", 8);
			write(1, s, ft_strlen(s));
			write(1, "': not a valid identifier\n", 26);
			return (1);
		}
	}
	more_export_parse(all, s, i);
	return (0);
}
