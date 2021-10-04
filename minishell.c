/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:56:01 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/04 05:01:09 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int i;

	if (!(array))
		return ;
	i = -1;
	while (array[++i])
	{
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
}

void	free_all(t_all *all, char *line_mask[])
{
	t_list *tmp;

	tmp = all->l_cmd;
	while (tmp)
	{
		all->cmd = (t_cmd*)tmp->content;
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

int    export_parse(t_all *all, char *s)
{
    int     i;
    t_env   *env;

    i = -1;
	if (s[0] && !ft_isalpha(s[0]) && s[0] != '_')
	{
		write(1, "export: `", 8);
		write(1, s, ft_strlen(s));
		write(1, "': not a valid identifier\n", 26);
		return (1);
	}
	while (s[++i] && s[i] != '=')
	{
		if (!ft_isalpha(s[i]) && !ft_isdigit(s[i]) && s[i] != '_')
		{
			write(1, "export: `", 8);
			write(1, s, ft_strlen(s));
			write(1, "': not a valid identifier\n", 26);
			return (1);
    	}
	}
	if (s[i] == '=')
	{
		env = (t_env*)malloc(sizeof(t_env));
		if (s[i])
		{
			env->name = ft_strndup(s, i);
			env->value = ft_strdup(s + i + 1);
		}
		else
		{
			env->name = ft_strdup(s);
			env->value = NULL;
		}
		ft_export_wa(all, env);
	}
	return (0);
}

int		main(int argc, char **argv, char **env)
{
	t_all	all;
	char *line_mask[2] = {0};

	if (argc > 1)
		return (0);
	(void)argv;
	ft_struct_bezero(&all, sizeof(t_all));
	parce_env(&all, env);
	all.envp = s_env(&all);
	g_child = 0;
	while (1)
	{
		all.error = 0;
		all.add = 1;
		line_mask[LINE] = s_readline(&all, "Minisheeesh-> ");
		if (line_mask[LINE] && line_mask[LINE][0] != '\0')
		{
			parse(&all, line_mask);
			find_var(&all, line_mask);
			parse(&all, line_mask);
			if (!all.error)
			{
				if (ft_strchr(line_mask[MASK], '|'))
				{
					all.pip = 1;
					get_pips(&all, line_mask, spl_nb(line_mask[MASK],'|') + 1);
				}
				else
				{
					all.pip = 0;
					all.nextin = 0;
					get_cmd(&all, line_mask, spl_nb(line_mask[MASK],' ') + 1);
					new_func(&all, all.l_cmd->content);
				}
			}
			else if (all.error)
				ft_putstr_fd("syntax error\n", 1);
		}
		// else if (!line_mask) //! what is this for?
		// {
		// 	printf("exit\n");
		// 	exit(0);
		// }
		free_all(&all, line_mask);
	}
	return (all.exits);
}