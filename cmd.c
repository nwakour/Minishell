/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:29:27 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/04 04:53:50 by tenshi           ###   ########.fr       */
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
		return (2);
}

int args_nb(char *ref)
{
	int i;
	int nb;

	i = 0;
	nb = 0;
	while (ref[i] != '\0')
	{
		if ((ref[i] == '1' && ref[i + 1] == '2')
		|| (ref[i] == '3' && ref[i + 1] == '4'))
			nb++;
		i++;
	}
	return nb;
}

void ft_split_mask(char *split_mask[][2], char **line_mask, char c)
{
	char **tmp_line;
	char **tmp_mask;
	int i;

    if (!line_mask[LINE] || !line_mask[MASK])
		return ;
	tmp_line = ft_split_ref(line_mask[LINE], line_mask[MASK], c);
	tmp_mask = ft_split(line_mask[MASK], c);
	i = -1;
	while (tmp_line[++i])
	{
		split_mask[i][LINE] = tmp_line[i];
		split_mask[i][MASK] = tmp_mask[i];
	}
	free(tmp_line);
	free(tmp_mask);
}

void	get_cmd(t_all *all, char **line_mask, int size)
{
	char	*split_mask[size][2];
	int		args;
	int		redirs;
	int		i;
	int		nb_args;

	nb_args = args_nb(line_mask[MASK]);
	remove_zero_ref(line_mask);
	ft_split_mask(split_mask, line_mask, ' ');
	redirs = str_n_set(line_mask[MASK], "><?=@");
	i = size;
	args = size - redirs - 1;
	if (args < 0)
		args = 0;
	if (!ft_struct_list(&all->l_cmd, (void**)&all->cmd, sizeof(t_cmd))
		|| size == 1)
		return ;
	if (redirs >= 0)
		all->cmd->f_name = (char **)ft_calloc(redirs + 1, sizeof(char*));
	if (args >= 0)
		all->cmd->arg = (char **)ft_calloc(args + 1, sizeof(char*));
	all->cmd->args = nb_args + args;
	while (size > 0 && split_mask[--size])
	{
		if (is_char_from_set(split_mask[size][MASK][0], "><?=@"))
		{
			split_mask[size][LINE][0] = split_mask[size][MASK][0];
			all->cmd->f_name[--redirs] = split_mask[size][LINE];
		}
		else if (size == 0 || (size != 0 && all->cmd->cmd == NULL && args == 0))
			all->cmd->cmd = split_mask[size][LINE];
		else
			all->cmd->arg[--args] = split_mask[size][LINE];
	}
	while (i && split_mask[i] && i--)
		free(split_mask[i][MASK]);
	i = -1;
	while(all->cmd->cmd && all->cmd->cmd[++i])
		 all->cmd->cmd[i] = ft_tolower(all->cmd->cmd[i]);
	s_heredoc(all, all->cmd);
	all->cmd->valid = check_cmd(all->cmd);
}

// char	*s_expand(char *line)
// {
// 	char	*s_line;
	
// }

char	*s_readdoc(t_all *all, char *limit, int expand)
{
	char	*line_mask[2];
	char	*file;
	int	fd;
	//int	pid;
	
	// expand = 0;
	all->add = 0;
	file = ft_strjoin("/tmp/s_", ft_itoa(all->hdoc));
	fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	//TODO: fork here
	// pid = fork();
	// if (pid < 0)
	// 	s_perror(all, "fork", 1);
	// g_child = 1;
	// if (pid == 0)
	// {
		line_mask[LINE] = s_readline(all, ">");
		parse_heredoc(all, line_mask, expand);
		while (line_mask[LINE] && ft_strcmp(line_mask[LINE], limit))
		{
			//TODO: check expand for env variables
			//write(2, "yoooo\n", 6);
			write(fd, line_mask[LINE], ft_strlen(line_mask[LINE]));
			write(fd, "\n", 1);
			line_mask[LINE] = s_readline(all, ">");
			parse_heredoc(all, line_mask, expand);
		}
		close(fd);
		//exit(0);
	// }
	// waitpid(pid, NULL, 0);
	// g_child = 0;
	// exit child here
	return (file);
}

void	s_heredoc(t_all *all, t_cmd *cmd)
{
	int	i;
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

void new_func(t_all *all, t_cmd *cmd)
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

void	execute_cmd(t_all *all, t_cmd *cmd)
{
	if (!cmd->cmd && cmd->fd)
		return ;
	else if (!(cmd->valid))
	{
		ft_putstr_fd(cmd->cmd, 1);
		ft_putstr_fd(": command not found\n", 1);
	}
	else if (!ft_strcmp(cmd->cmd, "echo"))
		ft_echo(all, cmd->arg);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd(all);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(all, cmd->arg[0], cmd->args);
	else if (!ft_strcmp(cmd->cmd, "export"))
		ft_export(all, cmd);
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(all);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(all, cmd);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		ft_unset(all, cmd);
}
