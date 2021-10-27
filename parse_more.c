/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_more.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:26:40 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/27 15:25:38 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	more_pips(t_all *all)
{
	t_list	*tmp;

	tmp = all->l_cmd;
	while (tmp->next)
	{
		fd_files(all, ((t_cmd *)tmp->content));
		s_cmd(all, tmp->content);
		tmp = tmp->next;
	}
	fd_files(all, ((t_cmd *)tmp->content));
	s_last(all, tmp->content);
	tmp = all->l_cmd;
	while (tmp)
	{
		s_wait(all, ((t_cmd *)tmp->content));
		tmp = tmp->next;
	}
}

void	get_pips(t_all *all, char **line_mask)
{
	t_line	*split_mask;
	int		i;

	all->pip = 1;
	split_mask = ft_split_mask(line_mask, '|');
	i = 0;
	while (split_mask && split_mask[i].line_mask[LINE])
		i++;
	all->pip = i;
	i = -1;
	all->hdoc = 0;
	all->nextin = 0;
	while (++i < all->pip)
	{
		get_cmd(all, split_mask[i].line_mask);
		free(split_mask[i].line_mask[MASK]);
		free(split_mask[i].line_mask[LINE]);
		all->inx++;
	}
	free(split_mask);
	more_pips(all);
}

static char	*var_value(t_all *all, char **line_mask, int *i)
{
	char	*var;
	char	*value;

	var = ft_strdup("$");
	while (line_mask[MASK][++(*i)] == 'v')
		var = ft_strjoinchar(var, line_mask[LINE][(*i)]);
	if (var && var[1] == '?' && var[2] == '\0')
		value = ft_itoa(all->exits);
	else
		value = check_env(all->l_env, var);
	free(var);
	return (value);
}

void	find_var(t_all *all, char **line_mask)
{
	char	*new_line;
	char	*value;
	int		i;

	i = -1;
	new_line = ft_strdup("");
	if (!new_line)
		return ;
	while (line_mask[MASK][++i] != '\0')
	{
		if (line_mask[MASK][i] == '$')
		{
			value = var_value(all, line_mask, &i);
			if (value)
				new_line = ft_strjoin_free(new_line, value);
			i--;
			free(value);
		}
		else
			new_line = ft_strjoinchar(new_line, line_mask[LINE][i]);
	}
	free(line_mask[LINE]);
	line_mask[LINE] = new_line;
}

void	parse_heredoc(t_all *all, char **line_mask, int expand)
{
	int	i;

	if (!line_mask[LINE] || line_mask[LINE][0] == '\0')
		return ;
	line_mask[MASK] = ft_strdup(line_mask[LINE]);
	i = -1;
	while (line_mask[LINE][++i])
	{
		if (expand && line_mask[LINE][i] == '$')
			handle_var_in_q(line_mask, &i);
		else
			line_mask[MASK][i] = TEXT;
	}
	find_var(all, line_mask);
}
