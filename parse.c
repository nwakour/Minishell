/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:05:04 by nwakour           #+#    #+#             */
/*   Updated: 2021/07/15 16:09:44 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse(t_all *all, char *line)
{
	int i;
	int ret;
	char *ref;

	i = -1;
	ref = strdup(line);
	if (line[skip_space(line)] == PIP)
	{
		all->error = 1;
		return(ref);
	}
	while (line[++i])
	{
		if ((ret = cor_char(line[i])) == BACK_S)
			skip_back_s(all, &i, &ref);
		else if (ret == OPEN_S_Q)
			handle_s_quotes(all, &i, &ref, line);
		else if (ret == OPEN_D_Q)
			handle_d_quotes(all, &i, &ref, line);
		else if (ret == FLAG)
			ref[i] = FLAG;
		else if (ret == SPICE)
			ref[i] = SPICE;
		else if (ret == PIP)
		{
			ref[i] = PIP;
			while (line[i + 1] == ' ')
				ref[++i] = SPICE;
			if (line[i + 1] == PIP)
			{
				all->error = 1;
				i++;
			}
		}
		// else if (ret == COLON)
		// {
		// 	ref[i] = COLON;
		// 	i++;
		// 	while (line[++i] == ' ')
		// 		ref[i] = SPICE;
		// 	if (line[i] == PIP || line[i] == COLON)
		// 		all->error = 1;
		// 	else
		// 		i--;
		// }
		else if (ret == VAR)
			handle_var(&i, &ref, line);
		else if (ret == GREAT)
		{
			if (ref[i + 1] == GREAT)
			{
				ref[i] = GREATER;
				ref[++i] = SKIP;
				while (line[i + 1] && line[i + 1] == ' ')
					ref[++i] = SKIP;
				if (line[i + 1] == GREAT)
				{
					all->error = 1;
					i++;
				}
			}
			else
			{
				ref[i] = GREAT;
				while (line[i + 1] && line[i + 1] == ' ')
					ref[++i] = SKIP;
				if (line[i + 1] == GREAT)
				{
					all->error = 1;
					i++;
				}
			}
		}
		else if (ret == LESS)
		{
			if (ref[i + 1] == LESS)
			{
				if (ref[i + 2 + skip_space(ref + i + 2)] == '\"' || ref[i + 2 + skip_space(ref + i + 2)] == '\'')
					ref[i] = LESSER_Q;
				else
					ref[i] = LESSER;
				ref[++i] = SKIP;
				while (line[i + 1] && line[i + 1] == ' ')
					ref[++i] = SKIP;
				if (line[i + 1] == LESS)
				{
					all->error = 1;
					i++;
				}
			}
			else
			{
				ref[i] = LESS;
				while (line[i + 1] && line[i + 1] == ' ')
					ref[++i] = SKIP;
				if (line[i + 1] == LESS)
				{
					all->error = 1;
					i++;
				}
			}
		}
		else
			ref[i] = TEXT;
	}
	return (ref);
}

void	get_pips(t_all *all, char *line, char *ref_line)
{
	char	**str;
	char	**ref;
	t_list *tmp;
	int i;

	str = ft_split_ref(line, ref_line, '|');
	ref = ft_split(ref_line, '|');
	i = 0;
	while (str[i] != NULL)
		i++;
	all->pip = i;
	i = -1;
	all->hdoc = 0;
	all->nextin = 0;
	while (str[++i] != NULL)
	{
		get_cmd(all, ft_strdup(str[i]), ft_strdup(ref[i]));
		all->inx++;
		// if (ft_strchr(tmp_ref->content, '|'))
		// 	get_pips(all, tmp->content, tmp_ref->content);
		// else
	}
	tmp = all->l_cmd;
	while (tmp->next)
	{
		fd_files(all, ((t_cmd*)tmp->content));
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
	all->l_cmd = NULL;
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

void	get_colons(t_all *all, char *line, char *line_ref)
{
	char	**str;
	char	**ref;
	t_list	*l_pip;
	t_list	*l_pip_ref;
	t_list	*tmp;
	t_list	*tmp_ref;

	l_pip = NULL;
	l_pip_ref = NULL;
	str = ft_split_ref(line, line_ref, '|');
	ref = ft_split(line_ref, '|');
	double_char_to_list(&l_pip, str);
	double_char_to_list(&l_pip_ref, ref);
	all->pip = ft_lstsize(l_pip);
	tmp = l_pip;
	tmp_ref = l_pip_ref;
	all->inx = 1;
	all->nextin = 0;
	while (tmp)
	{
		get_cmd(all, tmp->content, tmp_ref->content);
		tmp = tmp->next;
		tmp_ref = tmp_ref->next;
		all->inx++;
	}
	if (l_pip)
			ft_lstclear(&l_pip, &free_content);
	if (l_pip_ref)
			ft_lstclear(&l_pip_ref, &free_content);
	if (ref)
		free(ref);
	if (str)
		free(str);
}

char *find_var(t_all *all, char *line, char *line_ref)
{
	char *new_line;
	// char *tmp;
	char *var;
	char *value;
	int i;
	// int j;

	i = -1;
	// j = 0;
	new_line = ft_strdup("");
	while (line_ref[++i] != '\0')
	{
		if (line_ref[i] == '$')
		{
		
			// tmp = ft_strndup(line + j, i);
			var = ft_strdup("$");
			while (line_ref[++i] == 'v')
				var = ft_strjoinchar(var, line[i]);
			// j = i;
			if (var && var[1] == '?' && var[2] == '\0')
				value = ft_itoa(all->exits);
			else
				value = check_env(all->l_env, var);
			if (value)
				new_line = ft_strjoin(new_line, value);
			i--;
		}
		else
		{
			new_line = ft_strjoinchar(new_line, line[i]);
			// if (j != 0)
			// 	j++;
		}
	}
	free(line);
	return (new_line);
}