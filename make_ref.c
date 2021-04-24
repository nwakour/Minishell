/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:00:27 by nwakour           #+#    #+#             */
/*   Updated: 2021/04/24 15:28:45 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		cor_char(char c)
{
	if (c == '\"')
		return (OPEN_D_Q);
	else if (c == '\'')
		return (OPEN_S_Q);
	else if (c == '\\')
		return (BACK_S);
	else if (c == '$')
		return (VAR);
	else if (c == ';')
		return (COLON);
	else if (c == ' ')
		return (SPACE);
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

void	skip_back_s(t_all *all, int *i, char **ref_line)
{
	(*ref_line)[*i] = SKIP;
	*i += 1;
	if ((*ref_line)[*i] != '\0')
		(*ref_line)[*i] = TEXT;
	else
		all->error = 1;
}

void	skip_back_s_in_q(t_all *all, int *i, char **ref_line)
{
	if ((*ref_line)[(*i) + 1] == '$' || (*ref_line)[(*i) + 1] == '\\' || (*ref_line)[(*i) + 1] == '\"')
	{
		(*ref_line)[*i] = SKIP;
		*i += 1;
		if ((*ref_line)[*i] != '\0')
			(*ref_line)[*i] = TEXT;
		else
			all->error = 1;
	}
	else
		(*ref_line)[*i] = TEXT;
}

void	remove_zero_ref(char **s, char **ref)
{
	int		zeros;
	int		i;
	int		len;
	char	*new_s;
	char	*new_ref;
	int		j;

	if (!*s || !*ref || ((len = ft_strlen(*s)) != (int)ft_strlen(*ref)))
		return ;
	i = -1;
	zeros = 0;
	while ((*ref)[++i] != '\0')
	{
		if ((*ref)[i] == '0' || (*ref)[i] == '1' || (*ref)[i] == '2' || (*ref)[i] == '3' || (*ref)[i] == '4')
			zeros++;
	}
	if (!zeros)
		return ;
	len -= zeros;
	i = -1;
	if (!(new_s = ((char*)malloc((len) * sizeof(char)))))
		return ;
	if (!(new_ref = ((char*)malloc((len) * sizeof(char)))))
		return ;
	j = 0;
	while ((*s)[++i] != '\0')
	{
		if ((*ref)[i] != '0' && (*ref)[i] != '1' && (*ref)[i] != '2' && (*ref)[i] != '3' && (*ref)[i] != '4')
		{
			new_ref[j] = (*ref)[i];
			if ((*ref)[i] == 's')
				new_s[j] = ' ';
			else
				new_s[j] = (*s)[i];
			j++;
		}
	}
	new_ref[j] = '\0';
	new_s[j] = '\0';
	// if (*s)
	// 	free(*s);
	// if (*ref)
	// 	free(*ref);
	*s = new_s;
	*ref = new_ref;
}
