/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:44:20 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/29 15:44:21 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cont_remove_zero_ref(char **line_mask, char **nw_line_mask, int i)
{
	while (line_mask[LINE][++i] != '\0')
	{
		if (!is_char_from_set(line_mask[MASK][i], "01234"))
		{
			if (line_mask[MASK][i] == 's')
				nw_line_mask[LINE] = ft_strjoin_char(nw_line_mask[LINE], ' ');
			else if (nw_line_mask[LINE]
				&& is_char_from_set(line_mask[MASK][i], "<>!?")
				&& i != 0 && line_mask[MASK][i - 1] != ' ')
			{
				nw_line_mask[MASK] = ft_strjoin_char(nw_line_mask[MASK], ' ');
				nw_line_mask[LINE] = ft_strjoin_char(nw_line_mask[LINE], ' ');
				nw_line_mask[LINE] = ft_strjoin_char(nw_line_mask[LINE],
						line_mask[LINE][i]);
			}
			else
				nw_line_mask[LINE] = ft_strjoin_char(nw_line_mask[LINE],
						line_mask[LINE][i]);
			nw_line_mask[MASK] = ft_strjoin_char(nw_line_mask[MASK],
					line_mask[MASK][i]);
		}
	}
}

void	remove_zero_ref(char **line_mask)
{
	char	*nw_line_mask[2];

	if (!line_mask[LINE] || !str_n_set(line_mask[MASK], "01234"))
		return ;
	nw_line_mask[LINE] = NULL;
	nw_line_mask[MASK] = NULL;
	cont_remove_zero_ref(line_mask, nw_line_mask, -1);
	free(line_mask[LINE]);
	free(line_mask[MASK]);
	line_mask[LINE] = nw_line_mask[LINE];
	line_mask[MASK] = nw_line_mask[MASK];
}
