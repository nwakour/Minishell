/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:22:49 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/06 02:22:50 by tenshi           ###   ########.fr       */
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
