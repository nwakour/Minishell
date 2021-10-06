/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:56:01 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/06 04:38:15 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_all(t_all *all, char *line_mask[])
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
		}
		tmp = tmp->next;
	}
	ft_lstclear(&all->l_cmd, &free_content);
	free(line_mask[LINE]);
	line_mask[LINE] = NULL;
	free(line_mask[MASK]);
	line_mask[MASK] = NULL;
	all->l_cmd = NULL;
	all->cmd = NULL;
}

static void	s_dup(t_cmd *cmd)
{
	dup2(cmd->infd, 0);
	dup2(cmd->fd, 1);
	if (cmd->fd > 1)
		close(cmd->fd);
	if (cmd->infd > 1)
		close(cmd->infd);
}

static void	new_func(t_all *all, t_cmd *cmd)
{
	cmd->in = dup(0);
	cmd->out = dup(1);
	fd_files(all, cmd);
	if (!cmd->exec)
		exit(all->exits);
	if (cmd->valid == 1 && cmd->exec)
	{
		s_dup(cmd);
		execute_cmd(all, cmd);
		dup2(cmd->in, 0);
		dup2(cmd->out, 1);
	}
	else if (cmd->valid == 2 && cmd->exec)
		s_last(all, cmd);
	else if (!cmd->valid)
	{
		write(2, "\0", 1);
		write(2, ": command not found\n", 20);
		all->error = 127;
	}
}

static void	parse_exec(t_all *all, char **line_mask)
{
	if (line_mask[LINE] && line_mask[LINE][0] != '\0')
	{
		parse(all, line_mask);
		find_var(all, line_mask);
		parse(all, line_mask);
		if (!all->error)
		{
			if (ft_strchr(line_mask[MASK], '|'))
				get_pips(all, line_mask);
			else
			{
				all->pip = 0;
				all->nextin = 0;
				get_cmd(all, line_mask);
				new_func(all, all->l_cmd->content);
			}
		}
		else if (all->error)
			ft_putstr_fd("syntax error\n", 1);
		else if (!line_mask[LINE])
		{
			printf("exit\n");
			exit(0);
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
	while (1)
	{
		all.error = 0;
		all.add = 1;
		line_mask[LINE] = s_readline(&all, "Minisheeesh-> ");
		parse_exec(&all, line_mask);
		free_all(&all, line_mask);
	}
	return (all.exits);
}
