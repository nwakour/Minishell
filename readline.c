/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 16:54:26 by hmahjour          #+#    #+#             */
/*   Updated: 2021/10/25 19:56:09 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	s_handle_c(int sig)
{
	t_all	*all;

	all = get_exit__ptr(NULL);
	(void)sig;
	if (g_child == 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		all->exits = 1;
	}
}

static void	s_handle_q(int sig)
{
	(void)sig;
	if (g_child == 0)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

char	*s_readline(t_all *all, char *prompt, int doc)
{
	char	*line_mask;

	signal(SIGINT, s_handle_c);
	signal(SIGQUIT, s_handle_q);
	line_mask = readline(prompt);
	if (line_mask && line_mask[0] != '\0' && all->add)
	{
		add_history(line_mask);
		return (line_mask);
	}
	if (!line_mask && !doc)
	{
		write(1, "exit\n", 6);
		exit(all->exits);
	}
	return (line_mask);
}
