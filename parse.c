/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:05:04 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/04 04:27:20 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_great(t_all *all, char **line_mask, int *i)
{
	if (line_mask[MASK][(*i) + 1] == GREAT)
	{
		line_mask[MASK][(*i)] = GREATER;
		line_mask[MASK][++(*i)] = SKIP;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == GREAT)
		{
			all->error = 1;
			(*i)++;
		}
	}
	else
	{
		line_mask[MASK][(*i)] = GREAT;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == GREAT)
		{
			all->error = 1;
			(*i)++;
		}
	}
}

void	handle_less(t_all *all, char **line_mask, int *i)
{
	if (line_mask[MASK][(*i) + 1] == LESS)
	{
		if (line_mask[MASK][(*i) + 2 + skip_space(line_mask[MASK] + (*i) + 2)] == '\"'
			|| line_mask[MASK][(*i) + 2 + skip_space(line_mask[MASK] + (*i) + 2)] == '\'')
			line_mask[MASK][(*i)] = LESSER_Q;
		else
			line_mask[MASK][(*i)] = LESSER;
		line_mask[MASK][++(*i)] = SKIP;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == LESS)
		{
			all->error = 1;
			(*i)++;
		}
	}
	else
	{
		line_mask[MASK][(*i)] = LESS;
		while (line_mask[LINE][(*i) + 1] && line_mask[LINE][(*i) + 1] == ' ')
			line_mask[MASK][++(*i)] = SKIP;
		if (line_mask[LINE][(*i) + 1] == LESS)
		{
			all->error = 1;
			(*i)++;
		}
	}
}

void    parse(t_all *all, char **line_mask)
{
	int i;
	int ret;

	i = -1;
	if (line_mask[MASK])
		free(line_mask[MASK]);
	line_mask[MASK] = strdup(line_mask[LINE]);
	if (line_mask[LINE][skip_space(line_mask[LINE])] == PIP)
	{
		all->error = 1;
		return ;
	}
	while (line_mask[LINE][++i])
	{
		if ((ret = cor_char(line_mask[LINE][i])) == BACK_S)
			skip_back_s(all, line_mask[MASK], &i);
		else if (ret == OPEN_S_Q)
			handle_s_quotes(all, line_mask, &i);
		else if (ret == OPEN_D_Q)
			handle_d_quotes(all, line_mask, &i);
		else if (ret == FLAG)
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
	}
}

void	get_pips(t_all *all, char **line_mask, int size)
{
	char	*split_mask[size][2];
	t_list	*tmp;
	int i;

	ft_split_mask(split_mask, line_mask, '|');
	all->pip = size;
	i = -1;
	all->hdoc = 0;
	all->nextin = 0;
	while (++i < size)
	{
		get_cmd(all, split_mask[i], spl_nb(split_mask[i][MASK],' ') + 1);
		free(split_mask[i][MASK]);
		free(split_mask[i][LINE]);
		all->inx++;
	}
	tmp = all->l_cmd;
	while (tmp->next)
	{
		fd_files(all, ((t_cmd*)tmp->content));
		// check cmd_exec
		s_cmd(all, tmp->content);
		//new_func(all, tmp->content);
		tmp = tmp->next;
	}
	fd_files(all, ((t_cmd*)tmp->content));
	if (((t_cmd*)tmp->content)->valid > 0)
	{
		s_last(all, tmp->content);
	}
	else
	{
		write(2, "\0", 1);
		write(2, ": command not found\n", 20);
		all->exits = 127;
	}
	// NEW
	tmp = all->l_cmd;
	while (tmp)
	{
		s_wait(all, ((t_cmd*)tmp->content));
		tmp = tmp->next;
	}

	// NEW
	// all->l_cmd = NULL;
	// if (l_colon)
	// 		ft_lstclear(&l_colon, &free_content);
	// if (l_colon_ref)
	// 		ft_lstclear(&l_colon_ref, &free_content);
	// if (ref)
	// {
	// 	free(ref);
	// 	ref = NULL;
	// }
	// if (str)
	// {
	// 	free(str);
	// 	str = NULL;
	// }
}

void	find_var(t_all *all, char **line_mask)
{
	char *new_line;
	char *var;
	char *value;
	int i;

	i = -1;
	new_line = ft_strdup("");
	if (!new_line)
		return ;
	while (line_mask[MASK][++i] != '\0')
	{
		if (line_mask[MASK][i] == '$')
		{
			var = ft_strdup("$");
			while (line_mask[MASK][++i] == 'v')
				var = ft_strjoinchar(var, line_mask[LINE][i]);
			if (var && var[1] == '?' && var[2] == '\0')
				value = ft_itoa(all->exits);
			else
				value = check_env(all->l_env, var);
			if (value)
				new_line = ft_strjoin(new_line, value);
			i--;
		}
		else
			new_line = ft_strjoinchar(new_line, line_mask[LINE][i]);
	}
	free(line_mask[LINE]);
	line_mask[LINE] = new_line;
}

void    parse_heredoc(t_all *all, char **line_mask, int expand)
{
	int i;

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
