/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:35:52 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/29 15:36:13 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_varrr(char **line_mask, int *i)
{
	if (ft_isalpha(line_mask[LINE][*i + 1]) ||
		(line_mask[LINE][*i + 1] == '_'))
	{
		line_mask[MASK][*i] = VAR;
		while (ft_isalnum(line_mask[LINE][++(*i)]) ||
		(line_mask[LINE][*i] == '_'))
			line_mask[MASK][*i] = 'v';
		--(*i);
	}
	else if (line_mask[LINE][*i + 1] == '\0' || line_mask[LINE][*i + 1] == ' '
			|| line_mask[LINE][*i + 1] == '$')
		line_mask[MASK][*i] = TEXT;
	else
	{
		line_mask[MASK][*i] = VAR;
		line_mask[MASK][++*i] = 'v';
	}
}

void	handle_var(t_all *all, char **line_mask, int *i)
{
	if (line_mask[LINE][*i + 1] == ' ')
		line_mask[MASK][*i] = TEXT;
	else if (line_mask[LINE][*i + 1] == '\"')
	{
		line_mask[MASK][*i] = SKIP;
		(*i)++;
		handle_d_quotes(all, line_mask, i);
	}
	else if (line_mask[LINE][*i + 1] == '\'')
	{
		line_mask[MASK][*i] = SKIP;
		(*i)++;
		handle_s_quotes(all, line_mask, i);
	}
	else
		handle_varrr(line_mask, i);
}
