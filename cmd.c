/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 11:29:27 by nwakour           #+#    #+#             */
/*   Updated: 2021/09/15 14:40:06 by nwakour          ###   ########.fr       */
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

char	***ft_split_mask(char **line_mask, char c)
{
	char **tmp_line;
	char **tmp_mask;
	char ***split_mask;
	int i;

    if (!line_mask)
        return (NULL);
	tmp_line = ft_split_ref(line_mask[LINE], line_mask[MASK], c);
	tmp_mask = ft_split(line_mask[MASK], c);
	i = 0;
	while (tmp_line[i])
		i++;
	split_mask = (char ***)malloc(sizeof(char**) * (i + 1));
	if(!split_mask)
		return (NULL);
	i = -1;
	while (tmp_line[++i])
	{
		split_mask[i] = (char **)malloc(sizeof(char*) * 2);
		if (!split_mask[i])
			return (NULL);
		split_mask[i][LINE] = tmp_line[i];
		split_mask[i][MASK] = tmp_mask[i];
	}
	split_mask[i] = NULL;
	return (split_mask);
}

void	get_cmd(t_all *all, char **line_mask)
{
	char	***split_mask;
	int		args;
	int		redirs;
	int		i;
	int		nb_args;

	find_var(all, line_mask);
	parse(all, line_mask);
	nb_args = args_nb(line_mask[MASK]);
	line_mask = remove_zero_ref(line_mask);
	split_mask = ft_split_mask(line_mask, ' ');
	redirs = str_n_set(line_mask[MASK], "><?=@");
	args = 0;
	while (split_mask[args])
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
	while (i > 0 && split_mask[--i])
	{
		if (is_char_from_set(split_mask[i][MASK][0], "><?=@"))
		{
			split_mask[i][LINE][0] = split_mask[i][MASK][0];
			all->cmd->f_name[--redirs] = split_mask[i][LINE];
		}
		else if (i == 0 || (i != 0 && all->cmd->cmd == NULL && args == 0))
			all->cmd->cmd = split_mask[i][LINE];
		else
			all->cmd->arg[--args] = split_mask[i][LINE];
	}
	i = -1;
	while (split_mask[++i])
		free(split_mask[i]);
	free(split_mask);
	i = -1;
	while(all->cmd->cmd && all->cmd->cmd[++i])
		 all->cmd->cmd[i] = ft_tolower(all->cmd->cmd[i]);
	s_heredoc(all, all->cmd);
	all->cmd->valid = check_cmd(all->cmd);
	// free(str);
	// free(str_ref);
}

// char	*s_expand(char *line)
// {
// 	char	*s_line;
	
// }

char	*s_readdoc(t_all *all, char *limit, int expand)
{
	char	**line_mask;
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
		line_mask = s_readline(all, ">");
		parse_heredoc(all, line_mask, expand);
		while (line_mask && ft_strcmp(line_mask[LINE], limit))
		{
			//TODO: check expand for env variables
			//write(2, "yoooo\n", 6);
			write(fd, line_mask[LINE], ft_strlen(line_mask[LINE]));
			write(fd, "\n", 1);
			line_mask = s_readline(all, ">");
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
	
	//TODO: check for cmd->exec for file errors before executing
	
	fd_files(all, cmd);
	if (cmd->valid == 1)
		execute_cmd(all, cmd);
	else if (cmd->valid == 2 && cmd->exec)
		s_last(all, cmd);
	else if (!cmd->valid)
	{
		write(2, "\0", 1);
		write(2, ": command not found\n", 20);
		all->error = 127;
	}
	// else if (cmd->exec && (all->inx == all->pip || all->pip == 0))
	// {
	// 	//printf("here\n");
	// 	s_last(all, cmd);
	// }
	// if (cmd->fd > 1)
	// 	close(cmd->fd);
	// i = -1;
	// while (str[++i])
	// 	free(str[i]);
	//free(str);
	// i = -1;
	// while (str_ref[++i])
	// 	free(str_ref[i]);
	//free(str_ref);
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
		ft_echo(all, cmd->arg, cmd->fd);
	}
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd(all, cmd->fd);
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
		all->exits = 0;
		if (all->pip)
			exit(0);
	}
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(all, cmd->fd);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(all, cmd);
		// all->ex = 1;
	else if (!ft_strcmp(cmd->cmd, "unset"))
	{
		if (cmd->arg[0])
		{
			i = -1;
			while (cmd->arg[++i])
				ft_unset(all, cmd->arg[i]);
		}
		all->exits = 0;
		if (all->pip)
			exit(0);
	}
	else
		read_data(all);
}
