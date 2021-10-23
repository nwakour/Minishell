/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:29:27 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/23 15:24:56 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_cmd(t_all *all, t_cmd *cmd)
{
	if ((!cmd->cmd && !cmd->f_name[0]) || (!cmd->cmd && all->empty_cmd))
		return (0);
	else if (!cmd->cmd)
		return (3);
	else if (!ft_strcmp(cmd->cmd, "echo"))
		return (1);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		return (1);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd->cmd, "export"))
		return (1);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		return (1);
	else if (!ft_strcmp(cmd->cmd, "env"))
		return (1);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		return (1);
	else
		return (2);
}

static int	args_nb(char *ref)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (ref[i] != '\0')
	{
		if ((ref[i] == '1' && ref[i + 1] == '2')
			|| (ref[i] == '3' && ref[i + 1] == '4'))
			nb++;
		i++;
	}
	return (nb);
}

static void	s_heredoc(t_all *all, t_cmd *cmd)
{
	int		i;
	char	*limit;
	char	**tmp;
	char	*name;

	tmp = cmd->f_name;
	i = 0;
	while (tmp[i])
	{
		if (tmp[i][0] == '@' || tmp[i][0] == '=')
		{
			limit = tmp[i] + 1;
			if (tmp[i][0] == '=')
				name = s_readdoc(all, limit, 1);
			else
				name = s_readdoc(all, limit, 0);
			free(tmp[i]);
			tmp[i] = ft_strjoin("<", name);
			all->hdoc++;
		}
		i++;
	}
}

static void	make_cmd(t_all *all, t_line *split_mask, char **line_mask, int args)
{
	int	redirs;
	int	i;

	i = -1;
	while (split_mask && split_mask[++i].line_mask[LINE])
		args++;
	redirs = str_n_set(line_mask[MASK], "><?=@");
	if (args < 0)
		args = 0;
	all->cmd->args += args;
	all->cmd->f_name = (char **)ft_calloc(redirs + 1, sizeof(char *));
	all->cmd->arg = (char **)ft_calloc(args + 1, sizeof(char *));
	while (i-- > 0)
	{
		if (is_char_from_set(split_mask[i].line_mask[MASK][0], "><?=@"))
		{
			split_mask[i].line_mask[LINE][0] = split_mask[i].line_mask[MASK][0];
			all->cmd->f_name[--redirs] = split_mask[i].line_mask[LINE];
		}
		else if (!all->empty_cmd && (i == 0
				|| (i != 0 && all->cmd->cmd == NULL && args == 0)))
			all->cmd->cmd = split_mask[i].line_mask[LINE];
		else if (args != 0)
			all->cmd->arg[--args] = split_mask[i].line_mask[LINE];
	}
}

void	get_cmd(t_all *all, char **line_mask)
{
	t_line	*split_mask;
	int		args;
	int		i;
	int		nb_args;

	nb_args = args_nb(line_mask[MASK]);
	remove_zero_ref(line_mask);
	split_mask = ft_split_mask(line_mask, ' ');
	if (!ft_struct_list(&all->l_cmd, (void **)&all->cmd, sizeof(t_cmd))
		|| !split_mask)
		return ;
	args = 0 - str_n_set(line_mask[MASK], "><?=@") - 1;
	all->cmd->args = nb_args;
	make_cmd(all, split_mask, line_mask, args);
	i = -1;
	while (split_mask && split_mask[++i].line_mask[LINE])
		free(split_mask[i].line_mask[MASK]);
	free(split_mask);
	i = -1;
	while (all->cmd->cmd && all->cmd->cmd[++i])
		 all->cmd->cmd[i] = ft_tolower(all->cmd->cmd[i]);
	s_heredoc(all, all->cmd);
	all->cmd->valid = check_cmd(all, all->cmd);
}
