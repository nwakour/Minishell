/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 16:54:26 by hmahjour          #+#    #+#             */
/*   Updated: 2021/07/14 15:59:20 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	s_handle_c()
{
	sigmanum = 1;	
}

void	s_readline(t_all *all, char **line, char *prompt)
{
	(void)all;
	signal(SIGINT, s_handle_c);
	signal(SIGQUIT, SIG_IGN);
	*line = readline(prompt);
	if (*line && **line)
		add_history(*line);
}
