/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:25:40 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/06 03:03:33 by tenshi           ###   ########.fr       */
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
