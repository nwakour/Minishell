/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:56:01 by nwakour           #+#    #+#             */
/*   Updated: 2021/11/04 16:50:20 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_all(t_all *all)
{
	t_list	*tmp;

	tmp = all->l_cmd;
	while (tmp)
	{
		all->cmd = (t_cmd *)tmp->content;
		if (all->cmd)
		{
			free(all->cmd->cmd);
			all->cmd->cmd = NULL;
			free_array(all->cmd->arg);
			all->cmd->arg = NULL;
			free_array(all->cmd->f_name);
			all->cmd->f_name = NULL;
			free(all->cmd->in);
			all->cmd->in = NULL;
			free(all->cmd->out);
			all->cmd->out = NULL;
		}
		tmp = tmp->next;
	}
	ft_lstclear(&all->l_cmd, &free_content);
}

static void	s_dup(t_all *all, t_cmd *cmd)
{
	dup2(cmd->infd, 0);
	dup2(cmd->fd, 1);
	if (cmd->fd > 1)
		close(cmd->fd);
	if (cmd->infd > 1)
		close(cmd->infd);
	execute_cmd(all, cmd);
	dup2(cmd->tmpin, 0);
	dup2(cmd->tmpout, 1);
	close(cmd->tmpin);
	close(cmd->tmpout);
}

static void	new_func(t_all *all, t_cmd *cmd)
{
	cmd->tmpin = dup(0);
	cmd->tmpout = dup(1);
	fd_files(all, cmd);
	if (!cmd->exec)
		return ;
	if (cmd->valid == 1 && cmd->exec)
	{
		get_files(0, 1, cmd);
		s_dup(all, cmd);
	}
	else if (cmd->valid == 2 && cmd->exec)
		s_last(all, cmd);
	else if (!cmd->valid)
	{
		write(2, "\0", 1);
		write(2, ": command not found\n", 20);
		all->exits = 127;
	}
}

static void	parse_exec(t_all *all, char **line_mask)
{
	if (line_mask[LINE] && line_mask[LINE][0] != '\0')
	{
		parse(all, line_mask, -1, 0);
		find_var(all, line_mask);
		parse(all, line_mask, -1, 0);
		if (!all->error)
		{
			if (ft_strchr(line_mask[MASK], '|'))
				get_pips(all, line_mask);
			else if (line_mask[MASK][0] != '\0')
			{
				all->pip = 0;
				all->nextin = 0;
				get_cmd(all, line_mask);
				new_func(all, all->l_cmd->content);
			}
		}
		else if (all->error)
		{
			all->exits = 258;
			ft_putstr_fd("syntax error\n", 1);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_all	all;
	char	*line_mask[2];

	if (argc > 1)
		return (0);
	(void)argv;
	ft_struct_bezero(&all, sizeof(t_all));
	parce_env(&all, env);
	all.envp = s_env(&all);
	g_child = 0;
	line_mask[MASK] = NULL;
	line_mask[LINE] = NULL;
	get_exit__ptr(&all);
	all.nextin = 0;
	while (1)
	{
		all.error = 0;
		all.add = 1;
		all.empty_cmd = 0;
		line_mask[LINE] = s_readline(&all, "Minisheeesh-> ", 0);
		parse_exec(&all, line_mask);
		free_all(&all);
		freeeee(&all, line_mask);
	}
	return (all.exits);
}
