/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:44:15 by nwakour           #+#    #+#             */
/*   Updated: 2021/11/04 16:50:35 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_n_set(char *str, char *set)
{
	int	i;
	int	j;
	int	nb;

	i = -1;
	nb = 0;
	if (!str || !set)
		return (0);
	while (str[++i])
	{
		j = -1;
		while (set[++j])
		{
			if (str[i] == set[j])
				nb++;
		}
	}
	return (nb);
}

int	skip_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	skip_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == c)
		i++;
	return (i);
}

char	*ft_strjoinchar(char *s, char c)
{
	int		i;
	char	*str;

	i = 0;
	while (s[i])
		i++;
	str = (char *)malloc(i + 2);
	if (!str)
		return (0);
	i = -1;
	while (s[++i])
		str[i] = s[i];
	str[i] = c;
	str[i + 1] = '\0';
	free(s);
	return (str);
}

void	freeeee(t_all *all, char *line_mask[])
{
	free(line_mask[LINE]);
	line_mask[LINE] = NULL;
	free(line_mask[MASK]);
	line_mask[MASK] = NULL;
	all->l_cmd = NULL;
	all->cmd = NULL;
}
