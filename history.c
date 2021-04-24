/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 12:10:46 by nwakour           #+#    #+#             */
/*   Updated: 2021/04/22 14:01:48 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_history(t_all *all, char *line)
{
	if (!all->l_history)
		all->l_history = lstnewc(ft_strdup(line));
	else
		lstadd_dlist(&all->l_history, lstnewc(ft_strdup(line)));
}
