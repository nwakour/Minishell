/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:05:04 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/06 03:05:31 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_great(t_all *all, char **line_mask, int *i)
{
	if (line_mask[MASK][(*i) + 1] == GREAT)
	{
		line_mask[MASK][(*i)] = GREATER;
		line_mask[MASK][++(*i)] = SKIP;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == GREAT && ++(*i))
			all->error = 1;
	}
	else
	{
		line_mask[MASK][(*i)] = GREAT;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == GREAT && ++(*i))
			all->error = 1;
	}
}

static void	handle_less(t_all *all, char **line_mask, int *i)
{
	if (line_mask[MASK][(*i) + 1] == LESS)
	{
		if (line_mask[MASK][(*i) + 2
			+ skip_space(line_mask[MASK] + (*i) + 2)] == '\"'
			|| line_mask[MASK][(*i) + 2
			+ skip_space(line_mask[MASK] + (*i) + 2)] == '\'')
			line_mask[MASK][(*i)] = LESSER_Q;
		else
			line_mask[MASK][(*i)] = LESSER;
		line_mask[MASK][++(*i)] = SKIP;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == LESS && ++(*i))
			all->error = 1;
	}
	else
	{
		line_mask[MASK][(*i)] = LESS;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == LESS && ++(*i))
			all->error = 1;
	}
}

static int	continue_parse(t_all *all, char **line_mask, int i, int ret)
{
	if (ret == FLAG)
		line_mask[MASK][i] = FLAG;
	else if (ret == SPICE)
		line_mask[MASK][i] = SPICE;
	else if (ret == PIP)
	{
		line_mask[MASK][i] = PIP;
		while (line_mask[LINE][i + 1] == ' ')
			line_mask[MASK][++i] = SPICE;
		if (line_mask[LINE][i + 1] == PIP)
		{
			all->error = 1;
			i++;
		}
	}
	else if (ret == VAR)
		handle_var(line_mask, &i);
	else if (ret == GREAT)
		handle_great(all, line_mask, &i);
	else if (ret == LESS)
		handle_less(all, line_mask, &i);
	else
		line_mask[MASK][i] = TEXT;
	return (i);
}

static int	cor_char(char c)
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

void	parse(t_all *all, char **line_mask)
{
	int		i;
	int		ret;

	i = -1;
	free(line_mask[MASK]);
	line_mask[MASK] = strdup(line_mask[LINE]);
	if (line_mask[LINE][skip_space(line_mask[LINE])] == PIP)
	{
		all->error = 1;
		return ;
	}
	while (line_mask[LINE][++i])
	{
		ret = cor_char(line_mask[LINE][i]);
		if (ret == BACK_S)
			skip_back_s(all, line_mask[MASK], &i);
		else if (ret == OPEN_S_Q)
			handle_s_quotes(all, line_mask, &i);
		else if (ret == OPEN_D_Q)
			handle_d_quotes(all, line_mask, &i);
		else
			i = continue_parse(all, line_mask, i, ret);
	}
}
