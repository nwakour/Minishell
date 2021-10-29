/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 10:57:41 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/29 15:35:41 by nwakour          ###   ########.fr       */
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
		while (ft_isalnum(line_mask[LINE][++(*i)]) ||
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

static void	handle_d_quotes_more(t_all *all, char **line_mask, int *i)
{
	line_mask[MASK][*i] = OPEN_D_Q + 48;
	while (line_mask[LINE][++(*i)] != '\0')
	{
		if (line_mask[LINE][*i] == VAR)
			handle_var_in_q(line_mask, i);
		else if (line_mask[LINE][*i] == '\"')
		{
			line_mask[MASK][*i] = CLOSE_D_Q + 48;
			return ;
		}
		else
			line_mask[MASK][*i] = TEXT;
	}
	(*i)--;
	all->error = 1;
}

void	handle_d_quotes(t_all *all, char **line_mask, int *i)
{
	if ((line_mask[LINE][(*i) + 1] == '\"'
		&& line_mask[LINE][(*i) + 2] == ' '
		&& line_mask[LINE][skip_space(line_mask[LINE] + (*i) + 3)
		+ (*i) + 3] != '\0'))
	{
		line_mask[MASK][(*i)] = SKIP;
		line_mask[MASK][++(*i)] = SKIP;
		line_mask[MASK][++(*i)] = 's';
		while (line_mask[LINE][++(*i)] == ' ')
			line_mask[MASK][(*i)] = SKIP;
		(*i)--;
		return ;
	}
	handle_d_quotes_more(all, line_mask, i);
}

static void	handle_s_quotes_more(t_all *all, char **line_mask, int *i)
{
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
	(*i)--;
	all->error = 1;
}

void	handle_s_quotes(t_all *all, char **line_mask, int *i)
{
	if ((line_mask[LINE][(*i) + 1] == '\''
		&& line_mask[LINE][(*i) + 2] == ' '
		&& line_mask[LINE][skip_space(line_mask[LINE] + (*i) + 3)
		+ (*i) + 3] != '\0'))
	{
		line_mask[MASK][(*i)] = SKIP;
		line_mask[MASK][++(*i)] = SKIP;
		line_mask[MASK][++(*i)] = 's';
		while (line_mask[LINE][++(*i)] == ' ')
			line_mask[MASK][(*i)] = SKIP;
		(*i)--;
		return ;
	}
	handle_s_quotes_more(all, line_mask, i);
}
