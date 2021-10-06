/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_more.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 02:14:09 by tenshi            #+#    #+#             */
/*   Updated: 2021/10/06 03:39:31 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_line	*ft_split_mask(char **line_mask, char c)
{
	t_line	*split_mask;
	char	**tmp_line;
	char	**tmp_mask;
	int		i;

	if (!line_mask[LINE] || !line_mask[MASK])
		return (NULL);
	tmp_line = ft_split_ref(line_mask[LINE], line_mask[MASK], c);
	tmp_mask = ft_split(line_mask[MASK], c);
	i = 0;
	while (tmp_line[i])
		i++;
	if (i)
		split_mask = (t_line *)ft_calloc(i + 1, sizeof(t_line));
	else
		split_mask = NULL;
	i = -1;
	while (split_mask && tmp_line[++i])
	{
		split_mask[i].line_mask[LINE] = tmp_line[i];
		split_mask[i].line_mask[MASK] = tmp_mask[i];
	}
	free(tmp_line);
	free(tmp_mask);
	return (split_mask);
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

char	*s_readdoc(t_all *all, char *limit, int expand)
{
	char	*line_mask[2];
	char	*file;
	int		fd;

	all->add = 0;
	file = ft_strjoin("/tmp/s_", ft_itoa(all->hdoc));
	fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	line_mask[LINE] = s_readline(all, ">");
	parse_heredoc(all, line_mask, expand);
	while (line_mask[LINE] && ft_strcmp(line_mask[LINE], limit))
	{
		write(fd, line_mask[LINE], ft_strlen(line_mask[LINE]));
		write(fd, "\n", 1);
		line_mask[LINE] = s_readline(all, ">");
		parse_heredoc(all, line_mask, expand);
	}
	close(fd);
	return (file);
}
