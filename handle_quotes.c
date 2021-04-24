/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 10:57:41 by nwakour           #+#    #+#             */
/*   Updated: 2021/04/24 15:28:50 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_var_in_q(int *i, char** ref_line, char *line)
{
	if (line[*i + 1] == ' ')
		(*ref_line)[*i] = TEXT;
	else  if ((line[*i + 1] >= 'A' && line[*i + 1] <= 'Z')
		|| (line[*i + 1] >= 'a' && line[*i + 1] <= 'z') || (line[*i + 1] == '_'))
	{
		(*ref_line)[*i] = VAR;
		while ((line[++*i] >= 'A' && line[*i] <= 'Z')
			|| (line[*i] >= 'a' && line[*i] <= 'z') || (line[*i] == '_'))
			(*ref_line)[*i] = 'v';
		--*i;
	}
	else
	{
		(*ref_line)[*i] = VAR;
		(*ref_line)[++*i] = 'v';
	}
}
void	handle_var(int *i, char **ref_line, char *line)
{
	if (line[*i + 1] == ' ')
		(*ref_line)[*i] = TEXT;
	else if (line[*i + 1] == '\\')
	{
		(*ref_line)[*i] = TEXT;
		// (*ref_line)[++*i] = SKIP;
	}
	else  if ((line[*i + 1] >= 'A' && line[*i + 1] <= 'Z')
		|| (line[*i + 1] >= 'a' && line[*i + 1] <= 'z') || (line[*i + 1] == '_'))
	{
		(*ref_line)[*i] = VAR;
		while ((line[++*i] >= 'A' && line[*i] <= 'Z')
			|| (line[*i] >= 'a' && line[*i] <= 'z') || (line[*i] == '_'))
			(*ref_line)[*i] = 'v';
		--*i;
	}
	else
	{
		(*ref_line)[*i] = VAR;
		(*ref_line)[++*i] = 'v';
	}
}
void	handle_d_quotes(t_all *all, int *i, char** ref_line, char *line)
{
	if (line[*i] == '\"' && line[(*i) + 1] == '\"' && (line[(*i) - 1] == ' ' || line[(*i) + 2] == ' '))
	{
		(*ref_line)[*i] = SKIP;
		(*ref_line)[++(*i)] = 's';
		return ;
	}
	(*ref_line)[*i] = OPEN_D_Q + 48;
	while (line[++*i] != '\0')
	{
		if (line[*i] == VAR)
		 	handle_var(i, ref_line, line);
		else if (line[*i] == '\\')
			skip_back_s_in_q(all, i, ref_line);
		else if (line[*i] == '\"')
		{
			(*ref_line)[*i] = CLOSE_D_Q + 48;
			return ;
		}
		else
			(*ref_line)[*i] = TEXT;
	}
	all->error = 1;
}

void	handle_s_quotes(t_all *all, int *i, char** ref_line, char *line)
{
	if (line[*i] == '\'' && line[(*i) + 1] == '\'' && (line[(*i) - 1] == ' ' || line[(*i) + 2] == ' '))
	{
		(*ref_line)[*i] = SKIP;
		(*ref_line)[++(*i)] = 's';
		return ;
	}
	(*ref_line)[*i] = OPEN_S_Q + 48;
	while (line[++*i] != '\0')
	{
		if (line[*i] == '\'')
		{
			(*ref_line)[*i] = CLOSE_S_Q + 48;
			return ;
		}
		else
			(*ref_line)[*i] = TEXT;
	}
	all->error = 1;
}
