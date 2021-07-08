/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:29:27 by nwakour           #+#    #+#             */
/*   Updated: 2021/07/08 18:01:59 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_cmd(t_cmd *cmd)
{
	if (!cmd->cmd)
		return (0);
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
		return (0);
}

// static void			ft_free(char **str)
// {
// 	while (*str != NULL)
// 	{
// 		free(*str);
// 		str++;
// 	}
// 	free(str);
// }

int args_nb(char *ref)
{
	int i;
	int nb;

	i = 0;
	nb = 0;
	while (ref[i] != '\0')
	{
		if ((ref[i] == '1' && ref[i + 1] == '2') || (ref[i] == '3' && ref[i + 1] == '4'))
			nb++;
		i++;
	}
	return nb;
}

void	get_cmd(t_all *all, char *line, char *ref_line)
{
	char	**str;
	char	**str_ref;
	int		args;
	int		redirs;
	int		i;
	int		nb_args;

	line = find_var(all, line, ref_line);
	ref_line = parse(all, line);
	nb_args = args_nb(ref_line);
	remove_zero_ref(&line, &ref_line);
	str = ft_split_ref(line, ref_line, ' ');
	str_ref = ft_split(ref_line, ' ');
	// flags = str_n_char(ref_line, '-');
	redirs = str_n_char(ref_line, '>');
	redirs += str_n_char(ref_line, '<');
	redirs += str_n_char(ref_line, '?');
	args = 0;
	while (str[args])
		args++;
	i = args;
	args = args - redirs - 1;
	if (args < 0)
		args = 0;
	if (!i || !ft_struct_list(&all->l_cmd, (void**)&all->cmd, sizeof(t_cmd)))
		return ;
	if (redirs >= 0)
	{
		all->cmd->f_name = (char**)(malloc((redirs + 1) * sizeof(char*)));
		all->cmd->f_name[redirs] = 0;
	}
	if (args >= 0)
	{
		all->cmd->arg = (char**)(malloc((args + 1) * sizeof(char*)));
		all->cmd->arg[args] = 0;
	}
	all->cmd->args = nb_args + args;
	while (i > 0 && str[--i])
	{
		if (str_ref[i][0] == '<' || str_ref[i][0] == '>' || str_ref[i][0] == '?')
		{
			if (str_ref[i][0] == '?')
			{
				all->cmd->f_name[--redirs] = ft_strdup(str[i]);
				all->cmd->f_name[redirs][0] = '?';
			}
			else
				all->cmd->f_name[--redirs] = ft_strdup(str[i]);
		}
		else if (i == 0 || (i != 0 && all->cmd->cmd == NULL && args == 0))
			all->cmd->cmd = ft_strdup(str[i]);
		else
			all->cmd->arg[--args] = ft_strdup(str[i]);
	}
	i = -1;
	while(all->cmd->cmd && all->cmd->cmd[++i])
		 all->cmd->cmd[i] = ft_tolower(all->cmd->cmd[i]);
	all->cmd->valid = check_cmd(all->cmd);
	//TODO: check for cmd->exec for file errors before executing
	fd_files(all, all->cmd);
	if (check_cmd(all->cmd))
		execute_cmd(all, all->cmd);
	else if (all->cmd->exec && all->inx < all->pip)
		s_cmd(all, all->cmd);
	else if (all->cmd->exec && all->inx == all->pip)
		s_last(all, all->cmd);
	if (all->cmd->fd > 1)
		close(all->cmd->fd);
	// i = -1;
	// while (str[++i])
	// 	free(str[i]);
	free(str);
	// i = -1;
	// while (str_ref[++i])
	// 	free(str_ref[i]);
	free(str_ref);
}

void	execute_cmd(t_all *all, t_cmd *cmd)
{
	int flag;
	int i;

	flag = 0;
	if (!cmd->cmd && cmd->fd)
		return ;
	else if (!(cmd->valid))
	{
		ft_putstr_fd(cmd->cmd, 1);
		ft_putstr_fd(": command not found\n", 1);
	}
	else if (!ft_strcmp(cmd->cmd, "echo"))
	{
		ft_echo(cmd->arg, cmd->fd);
	}
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd(cmd->fd);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(all, cmd->arg[0], cmd->args);
	else if (!ft_strcmp(cmd->cmd, "export"))
	{
		if (cmd->arg[0])
		{
			i = -1;
			while (cmd->arg[++i])
				export_parse(all, cmd->arg[i]);
		}
		else
			ft_export_na(all, cmd->fd);
	}
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(all, cmd->fd);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit();
		// all->ex = 1;
	else if (!ft_strcmp(cmd->cmd, "unset"))
	{
		if (cmd->arg[0])
		{
			i = -1;
			while (cmd->arg[++i])
				ft_unset(all, cmd->arg[i]);
		}
	}
	else
		read_data(all);
}
