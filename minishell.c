/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:56:01 by nwakour           #+#    #+#             */
/*   Updated: 2021/07/14 19:45:36 by hmahjour         ###   ########.fr       */
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

void	get_data(t_all *all, char *line, char *ref_line)
{
	if (!line)
		return ;
	if (ft_strchr(ref_line, ';'))
		get_colons(all, line, ref_line);
	else if (ft_strchr(ref_line, '|'))
		get_pips(all, line, ref_line);
	else
	 	get_cmd(all, line, ref_line);
}

int		main(int argc, char **argv, char **env)
{
	t_all	all;
	char *ref_line;
	char *line;

	if (argc > 1)
		return (0);
	(void)argv;
	(void)env;
	ft_struct_bezero(&all, sizeof(t_all));
	parce_env(&all, env);
	all.old = (struct termios *)malloc(sizeof(struct termios));
	all.tccmd = (t_tc_cmd*)malloc(sizeof(t_tc_cmd));
	init_term(all.tccmd);
	while (1)
	{
		sigmanum = 0;
		all.error = 0;
		line = ft_strdup("");
		//terminal(&all, &line, "Minishell->>> ");
		all.add = 1;
		s_readline(&all, &line, "Minisheeesh-> ");
		if (line && line[0] != '\0')
		{
			ref_line = parse(&all, line);
			//printf("ref = %s\n", ref_line);
			if (!all.error && line[0] != '\0')
			{
				if (ft_strchr(ref_line, '|'))
				{
					all.pip = 1; 
					get_pips(&all, line, ref_line);
				}
				else
				{
					all.pip = 0;
					all.nextin = 0;
					get_cmd(&all, line, ref_line);
					new_func(&all, all.l_cmd->content);
					all.l_cmd = NULL;
				}
			}
			else if (all.error)
			{
				
				ft_putstr_fd("syntax error\n", 1);
				
			}
		}
		else if (!line)
		{
			printf("exit\n");
			exit(0);
		}
		if (line && line[0] != '\0')
			lstadd_dlist(&all.l_history, lstnewc(ft_strdup(line)));
	}
	return (0);
}
