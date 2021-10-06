/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_more.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:19:57 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/06 03:34:03 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
