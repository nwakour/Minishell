/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:00:27 by nwakour           #+#    #+#             */
/*   Updated: 2021/09/15 14:14:15 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		cor_char(char c)
{
	if (c == '\"')
		return (OPEN_D_Q);
	else if (c == '\'')
		return (OPEN_S_Q);
	else if (c == '$')
		return (VAR);
	else if (c == ' ')
		return (SPICE);
	else if (c == '|')
		return (PIP);
	else if (c == '-')
		return (FLAG);
	else if (c == '>')
		return (GREAT);
	else if (c == '<')
		return (LESS);
	else
		return (TEXT);
}

void	skip_back_s(t_all *all, char *mask, int *i)
{
	mask[*i] = SKIP;
	*i += 1;
	if (mask[*i] != '\0')
		mask[*i] = TEXT;
	else
		all->error = 1;
}

void	skip_back_s_in_q(t_all *all, char *mask, int *i)
{
	if (mask[(*i) + 1] == '$' || mask[(*i) + 1] == '\\' || mask[(*i) + 1] == '\"')
	{
		mask[*i] = SKIP;
		*i += 1;
		if (mask[*i] != '\0')
			mask[*i] = TEXT;
		else
			all->error = 1;
	}
	else
		mask[*i] = TEXT;
}

// int	ft_find_from_set(char *str, char *set)
// {
// 	int i;
// 	int j;

// 	i = -1;
// 	while (str[++i])
// 	{
// 		j = -1;
// 		while (set[++j])
// 		{
// 			if (str[i] == set[j])
// 				return (1);
// 		}
// 	}
// 	return 0;
// }

int	is_char_from_set(char c, char *set)
{
	int i;

	i = -1;
	while (set[++i])
	{
		if (set[i] == c)
			return (1);
	}
	return (0);
}

char	**remove_zero_ref(char **line_mask)
{
	int		zeros;
	int		i;
	int		len;
	char	**new_line_mask;

	
	if (!line_mask)
		return (line_mask);
	i = -1;
	zeros = str_n_set(line_mask[MASK], "01234");
	if (!zeros)
		return (line_mask);
	len = ft_strlen(line_mask[MASK]) - zeros;
	i = -1;
	new_line_mask = (char **)malloc(sizeof(char *) * 2);
	if (!new_line_mask)
		return (line_mask);
	new_line_mask[LINE] = NULL;
	new_line_mask[MASK] = NULL;
	while (line_mask[LINE][++i] != '\0')
	{
		if (!is_char_from_set(line_mask[MASK][i], "01234"))
		{
			new_line_mask[MASK] = ft_strjoin_char(new_line_mask[MASK], line_mask[MASK][i]);
			if (line_mask[MASK][i] == 's')
				new_line_mask[LINE] = ft_strjoin_char(new_line_mask[LINE], ' ');
			else
				new_line_mask[LINE] = ft_strjoin_char(new_line_mask[LINE], line_mask[LINE][i]);
		}
	}
	
	return (new_line_mask);
}
