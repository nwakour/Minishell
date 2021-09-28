/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 16:54:26 by hmahjour          #+#    #+#             */
/*   Updated: 2021/09/13 15:41:55 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	s_handle_c()
{
	if (!g_child)
	{	//write(2, "here\n", 5);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	s_handle_q()
{
	//write(2, ft_itoa(g_child), ft_strlen(ft_itoa(g_child)));
	if (g_child)
	{
		//write(2, "here\n", 5);
		write(1, "Quit: 3\n", 8);
		rl_on_new_line();
		rl_replace_line("", 0);
		//rl_redisplay();
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

char	**s_readline(t_all *all, char *prompt)
{
	char **line_mask;

	signal(SIGINT, s_handle_c);
	signal(SIGQUIT, s_handle_q);
	line_mask = (char**)malloc(sizeof(char *) * 2);
	if (!line_mask)
		return (NULL);
	line_mask[LINE] = readline(prompt);
	line_mask[MASK] = NULL;
	if (line_mask[LINE] && line_mask[LINE][0] != '\0' && all->add)
	{
		add_history(line_mask[LINE]);
		return (line_mask);
	}
	if (!line_mask[LINE])
	{
		write(1, "exit", 5);
		exit(all->exits);
	}
	// if (line_mask[LINE][0] == '\0')
	// 	line_mask[LINE] = NULL;
	return (line_mask);
}
