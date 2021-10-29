/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:44:40 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/29 15:44:41 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(t_all *all)
{
	t_list	*tmp;

	tmp = all->l_env;
	while (tmp)
	{
		if (!(ft_strcmp(((t_env *)tmp->content)->name, "PATH")))
			return (ft_strdup(((t_env *)tmp->content)->value));
		tmp = tmp->next;
	}
	return (NULL);
}

char	**s_paths(t_all *all)
{
	char	**path;
	char	*ss;

	path = NULL;
	ss = find_path(all);
	if (ss)
		path = ft_split(ss, ':');
	return (path);
}
