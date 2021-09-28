/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:56:01 by nwakour           #+#    #+#             */
/*   Updated: 2021/09/13 15:43:05 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_data(t_all *all)
{
	t_list	*tmp;
	t_cmd	*cmd;
	int		i;

	tmp = all->l_cmd;
	while (tmp)
	{
		cmd = (t_cmd*)tmp->content;
		printf("cmd = %s\n", cmd->cmd);
		// i = -1;
		// while (cmd->flag[++i])
		// 	printf("flags = %s\n", cmd->flag[i]);
		i = -1;
		while (cmd->arg[++i])
			printf("args = %s\n", cmd->arg[i]);
		tmp = tmp->next;
	}
}

void    export_parse(t_all *all, char *s)
{
    int     i;
    t_env   *env;

    i = -1;
	while (s[++i] && s[i] != '=')
	{
		if (s[i] == 96 || s[i] < '0' || (s[i] > '9' && s[i] < 'A')
			|| (s[i] > 'Z' && s[i] < '_') || s[i] > 'z')
		{
			printf("export: `%s': not a valid identifier\n", s);
    		return ;
    	}
	}
	i = 0;
    while(s[i] && s[i] != '=')
        i++;
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

// void	get_data(t_all *all, char *line, char *ref_line)
// {
// 	if (!line)
// 		return ;
// 	if (ft_strchr(ref_line, ';'))
// 		get_colons(all, line, ref_line);
// 	else if (ft_strchr(ref_line, '|'))
// 		get_pips(all, line, ref_line);
// 	else
// 	 	get_cmd(all, line, ref_line);
// }

int		main(int argc, char **argv, char **env)
{
	t_all	all;
	char **line_mask;

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
		line_mask = s_readline(&all, "Minisheeesh-> ");
		if (line_mask && line_mask[LINE] && line_mask[LINE][0] != '\0')
		{
			parse(&all, line_mask);
			if (!all.error)
			{
				if (ft_strchr(line_mask[MASK], '|'))
				{
					all.pip = 1;
					get_pips(&all, line_mask);
				}
				else
				{
					all.pip = 0;
					all.nextin = 0;
					get_cmd(&all, line_mask);
					new_func(&all, all.l_cmd->content);
					all.l_cmd = NULL;
				}
			}
			else if (all.error)
				ft_putstr_fd("syntax error\n", 1);
		}
		else if (!line_mask)
		{
			printf("exit\n");
			exit(0);
		}
	}
	return (all.exits);
}