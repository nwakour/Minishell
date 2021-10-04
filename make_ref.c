/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:00:27 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/04 04:48:42 by tenshi           ###   ########.fr       */
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
	if (mask[(*i) + 1] == '$' || mask[(*i) + 1] == '\\'
		|| mask[(*i) + 1] == '\"')
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
	if (!set)
		return (0);
	while (set[++i])
	{
		if (set[i] == c)
			return (1);
	}
	return (0);
}

int spl_nb(char *s, char c)
{
	int i;
	int nb;

	i = -1;
	nb = 0;
	while (s[++i] && s[i] == c);
	if (!s[i])
		return (nb);
	while (s[++i])
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			nb++;
	}
	return (nb);
}

void 	remove_zero_ref(char **line_mask)
{
	int		i;
	char	*nw_line_mask[2] = {0};

	if (!line_mask[LINE] || !str_n_set(line_mask[MASK], "01234"))
		return ;
	i = -1;
	while (line_mask[LINE][++i] != '\0')
	{
		if (!is_char_from_set(line_mask[MASK][i], "01234"))
		{
			nw_line_mask[MASK] = ft_strjoin_char(nw_line_mask[MASK],
				line_mask[MASK][i]);
			if (line_mask[MASK][i] == 's')
				nw_line_mask[LINE] = ft_strjoin_char(nw_line_mask[LINE], ' ');
			else
				nw_line_mask[LINE] = ft_strjoin_char(nw_line_mask[LINE],
					line_mask[LINE][i]);
		}
	}
	free(line_mask[LINE]);
	free(line_mask[MASK]);
	line_mask[LINE] = nw_line_mask[LINE];
	line_mask[MASK] = nw_line_mask[MASK];
}
