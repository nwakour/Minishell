/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 10:57:41 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/06 01:07:47 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_var_in_q(char **line_mask, int *i)
{
	if (line_mask[LINE][*i + 1] == ' ')
		line_mask[MASK][*i] = TEXT;
	else if (ft_isalpha(line_mask[LINE][*i + 1]) ||
		(line_mask[LINE][*i + 1] == '_'))
	{
		line_mask[MASK][*i] = VAR;
		while (ft_isalpha(line_mask[LINE][++(*i)]) ||
		(line_mask[LINE][*i] == '_'))
			line_mask[MASK][*i] = 'v';
		--(*i);
	}
	else
	{
		line_mask[MASK][*i] = VAR;
		line_mask[MASK][++*i] = 'v';
	}
}

void	handle_var(char **line_mask, int *i)
{
	if (line_mask[LINE][*i + 1] == ' ')
		line_mask[MASK][*i] = TEXT;
	else if (line_mask[LINE][*i + 1] == '\\')
		line_mask[MASK][*i] = TEXT;
	else if (ft_isalpha(line_mask[LINE][*i + 1]) ||
		(line_mask[LINE][*i + 1] == '_'))
	{
		line_mask[MASK][*i] = VAR;
		while (ft_isalpha(line_mask[LINE][++(*i)]) ||
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

void	handle_d_quotes(t_all *all, char **line_mask, int *i)
{
	if (line_mask[LINE][*i] == '\"' && line_mask[LINE][(*i) + 1] == '\"'
		&& (((*i != 0) && (line_mask[LINE][(*i) - 1] == ' '))
		|| (line_mask[LINE][(*i) + 1]
		&& line_mask[LINE][(*i) + 2] && line_mask[LINE][(*i) + 2] == ' ')))
	{
		line_mask[MASK][*i] = SKIP;
		line_mask[MASK][++(*i)] = 's';
		return ;
	}
	line_mask[MASK][*i] = OPEN_D_Q + 48;
	while (line_mask[LINE][++(*i)] != '\0')
	{
		if (line_mask[LINE][*i] == VAR)
			handle_var_in_q(line_mask, i);
		else if (line_mask[LINE][*i] == '\\')
			skip_back_s_in_q(all, line_mask[MASK], i);
		else if (line_mask[LINE][*i] == '\"')
		{
			line_mask[MASK][*i] = CLOSE_D_Q + 48;
			return ;
		}
		else
			line_mask[MASK][*i] = TEXT;
	}
	all->error = 1;
}

void	handle_s_quotes(t_all *all, char **line_mask, int *i)
{
	if (line_mask[LINE][*i] == '\'' && line_mask[LINE][(*i) + 1] == '\''
		&& (((*i != 0) && (line_mask[LINE][(*i) - 1] == ' '))
		|| (line_mask[LINE][(*i) + 1]
		&& line_mask[LINE][(*i) + 2] && line_mask[LINE][(*i) + 2] == ' ')))
	{
		line_mask[MASK][*i] = SKIP;
		line_mask[MASK][++(*i)] = 's';
		return ;
	}
	line_mask[MASK][*i] = OPEN_S_Q + 48;
	while (line_mask[LINE][++(*i)] != '\0')
	{
		if (line_mask[LINE][*i] == '\'')
		{
			line_mask[MASK][*i] = CLOSE_S_Q + 48;
			return ;
		}
		else
			line_mask[MASK][*i] = TEXT;
	}
	all->error = 1;
}
