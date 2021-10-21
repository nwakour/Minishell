/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:05:04 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/21 16:14:23 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_great(t_all *all, char **mline, int *i)
{
	if (mline[MASK][(*i) + 1] == GREAT)
	{
		mline[MASK][(*i)] = GREATER;
		mline[MASK][++(*i)] = SKIP;
		while (mline[LINE][(*i) + 1] && mline[LINE][(*i) + 1] == ' ')
			mline[MASK][++(*i)] = SKIP;
		if ((mline[LINE][(*i) + 1] == GREAT || mline[LINE][(*i) + 1] == LESS
			|| mline[LINE][(*i) + 1] == PIP || mline[LINE][(*i) + 1] == '\0')
			&& ++(*i))
			all->error = 1;
	}
	else
	{
		mline[MASK][(*i)] = GREAT;
		while (mline[LINE][(*i) + 1] && mline[LINE][(*i) + 1] == ' ')
			mline[MASK][++(*i)] = SKIP;
		if ((mline[LINE][(*i) + 1] == GREAT || mline[LINE][(*i) + 1] == LESS
			|| mline[LINE][(*i) + 1] == PIP || mline[LINE][(*i) + 1] == '\0')
			&& ++(*i))
			all->error = 1;
	}
}

static void	handle_less(t_all *all, char **mline, int *i)
{
	if (mline[MASK][(*i) + 1] == LESS)
	{
		if (mline[MASK][(*i) + 2
			+ skip_space(mline[MASK] + (*i) + 2)] == '\"'
			|| mline[MASK][(*i) + 2
			+ skip_space(mline[MASK] + (*i) + 2)] == '\'')
			mline[MASK][(*i)] = LESSER_Q;
		else
			mline[MASK][(*i)] = LESSER;
		mline[MASK][++(*i)] = SKIP;
		while (mline[LINE][(*i) + 1] && mline[LINE][(*i) + 1] == ' ')
			mline[MASK][++(*i)] = SKIP;
		if ((is_char_from_set(mline[LINE][(*i) + 1], "<>|")
			|| mline[LINE][(*i) + 1] == '\0') && ++(*i))
			all->error = 1;
	}
	else
	{
		mline[MASK][(*i)] = LESS;
		while (mline[LINE][(*i) + 1] && mline[LINE][(*i) + 1] == ' ')
			mline[MASK][++(*i)] = SKIP;
		if ((is_char_from_set(mline[LINE][(*i) + 1], "<>|")
			|| mline[LINE][(*i) + 1] == '\0') && ++(*i))
			all->error = 1;
	}
}

static int	continue_parse(t_all *all, char **mline, int i, int ret)
{
	if (ret == FLAG)
		mline[MASK][i] = FLAG;
	else if (ret == SPICE)
		mline[MASK][i] = SPICE;
	else if (ret == PIP)
	{
		mline[MASK][i] = PIP;
		while (mline[LINE][i + 1] == ' ')
			mline[MASK][++i] = SPICE;
		if (mline[LINE][i + 1] == PIP || mline[LINE][i + 1] == '\0')
		{
			all->error = 1;
			i++;
		}
	}
	else if (ret == VAR)
		handle_var(all, mline, &i);
	else if (ret == GREAT)
		handle_great(all, mline, &i);
	else if (ret == LESS)
		handle_less(all, mline, &i);
	else
		mline[MASK][i] = TEXT;
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

void	parse(t_all *all, char **mline)
{
	int		i;
	int		ret;

	i = -1;
	free(mline[MASK]);
	mline[MASK] = strdup(mline[LINE]);
	if (mline[LINE][skip_space(mline[LINE])] == PIP)
	{
		all->error = 1;
		return ;
	}
	while (mline[LINE][++i])
	{
		ret = cor_char(mline[LINE][i]);
		if (ret == BACK_S)
			skip_back_s(all, mline[MASK], &i);
		else if (ret == OPEN_S_Q)
			handle_s_quotes(all, mline, &i);
		else if (ret == OPEN_D_Q)
			handle_d_quotes(all, mline, &i);
		else
			i = continue_parse(all, mline, i, ret);
		if (i != 0 && mline[LINE][i] == '\0')
			return ;
	}
}
