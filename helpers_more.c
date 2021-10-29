/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:44:07 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/29 15:44:08 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_cmd(t_all *all, char **mline)
{
	int	i;

	i = skip_space(mline[LINE]);
	if (mline[LINE][i] == '\'' && mline[LINE][i + 1]
		&& mline[LINE][i + 1] == '\'' && mline[LINE][i + 2] == ' ')
		all->empty_cmd = 1;
	else if (mline[LINE][i] == '\"' && mline[LINE][i + 1]
		&& mline[LINE][i + 1] == '\"' && mline[LINE][i + 2] == ' ')
		all->empty_cmd = 1;
}

void	free_content(void *content)
{
	if ((char *)content)
	{
		free((char *)content);
		content = 0;
	}
}

int	is_char_from_set(char c, char *set)
{
	int	i;

	i = -1;
	if (!set)
		return (0);
	while (set[++i])
	{
		if (set[i] == c)
			return (1);
	}
	return (0);
}

void	free_array(char **array)
{
	int	i;

	if (!(array))
		return ;
	i = -1;
	while (array[++i])
	{
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
}
