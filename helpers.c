/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:25:40 by nwakour           #+#    #+#             */
/*   Updated: 2021/04/24 15:52:17 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		str_n_char(char *str, char c)
{
	int i;
	int nb;

	i = -1;
	nb = 0;
	while (str[++i])
	{
		if (str[i] == c)
			nb++;
	}
	return (nb);
}

int			skip_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int			skip_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == c)
		i++;
	return (i);
}

char	*ft_strjoinchar(char *s, char c)
{
	int		i;
	char	*str;
	i = 0;
	while (s[i])
		i++;
	if (!(str = (char *)malloc(i + 2)))
		return (0);
	i = -1;
	while (s[++i])
		str[i] = s[i];
	str[i] = c;
	str[i + 1] = '\0';
	free(s);
	return (str);
}

void	free_content(void *content)
{
	if ((char *)content)
	{
		free((char *)content);
		content = 0;
	}
}

void	free_char(char **s)
{
	int i;
	char *ss;

	ss = *s;
	i = ft_strlen(*s);
	free(ss);
	while (i >= 0)
	{
		ss[i] = '\0';
		i--;
	}
}

void	double_char_to_list(t_list **list, char **str)
{
	int		i;
	int		len;

	len = 0;
	while (str[len])
		len++;
	i = -1;
	while (++i < len)
	{
		if (!*list)
			*list = ft_lstnew(str[i]);
		else
			ft_lstadd_back(list, ft_lstnew(str[i]));
	}
}

void	lstadd_dlist(t_dlist **alst, t_dlist *new)
{
	// t_dlist	*last;

	if (!new)
		return ;
	// last = *alst;
	new->prev = NULL;
	if ((*alst) == NULL)
		(*alst) = new;
	else
	{
		new->next = *alst;
		(*alst)->prev = new;
		*alst = new;
	}
	// if (alst)
	// {
	// 	new->next = *alst;
	// 	(*alst)->prev = new;
	// 	*alst = new;
	// }
	// else if (new)
	// 	*alst = new;
}

void	next_node(t_dlist **list)
{
	if (!*list)
		return ;
	if ((*list)->next != NULL)
		(*list) = (*list)->next;
}
void	prev_node(t_dlist **list)
{
	if (!*list)
		return ;
	if ((*list)->prev != NULL)
		(*list) = (*list)->prev;
}

t_dlist	*lstnewc(void *cam)
{
	t_dlist	*p;
	p = (t_dlist*)malloc(sizeof(t_dlist));
	if (!p)
		return (NULL);
	p->cam = cam;
	p->next = NULL;
	p->prev = NULL;
	return (p);
}
