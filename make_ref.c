/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 04:31:32 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/06 04:31:34 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	remove_zero_ref(char **line_mask)
{
	int		i;
	char	*nw_line_mask[2];

	if (!line_mask[LINE] || !str_n_set(line_mask[MASK], "01234"))
		return ;
	nw_line_mask[LINE] = NULL;
	nw_line_mask[MASK] = NULL;
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
