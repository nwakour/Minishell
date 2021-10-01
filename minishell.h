/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:59:03 by nwakour           #+#    #+#             */
/*   Updated: 2021/09/15 14:41:58 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <termcap.h>
# include <termios.h>
# include <signal.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <time.h>
# include <readline/readline.h>
# include <readline/history.h>

# define OPEN_S_Q 1
# define CLOSE_S_Q 2
# define OPEN_D_Q 3
# define CLOSE_D_Q 4
# define BACK_S 92
# define VAR 36
# define TEXT 97
# define PIP 124
# define SPICE 32
# define FLAG 45
# define SKIP 48
# define GREAT 62
# define LESS 60
# define GREATER 63
# define LESSER 61
# define LESSER_Q 64

# define LINE 0
# define MASK 1

int	g_child;

typedef struct		s_cmd
{
	char	*cmd;
	char	**arg;
	int		infd;
	int		fd;
	char	in;
	char	out;
	int		append;
	int		link;
	char	**f_name;
	int		args;
	int		valid;
	int		pid;
	int		exec;
}					t_cmd;

typedef struct		s_env
{
	char *name;
	char *value;
}					t_env;

typedef struct          s_dlist
{
    void                *cam;
    struct s_dlist      *prev;
    struct s_dlist      *next;
}                       t_dlist;

typedef struct		s_tc_cmd
{
	char			*cl;
	char			*dl;
	char			*le;
	char			*nd;

}					t_tc_cmd;

typedef struct		s_all
{
	t_list	*l_cmd;
	t_list	*l_env;
	t_env	*env;
	t_cmd	*cmd;
	t_tc_cmd	*tccmd;
	char	**envp;
	int		error;
	int		exits;
	int		pip;
	int		inx;
	int		nextin;
	int		hdoc;
	int		add;
}					t_all;

/*				minishell			*/
void	read_data(t_all *all);
int    export_parse(t_all *all, char *s);
void	get_data(t_all *all, char* line, char *ref_line);

/*				builtins			*/
void	ft_echo(t_all *all, char **arg);
void	ft_cd(t_all *all, char* path, int args);
void	ft_pwd(t_all *all);
void	ft_export(t_all *all, t_cmd *cmd);
void	ft_export_na(t_all *all);
void	ft_export_wa(t_all *all, t_env *var);
void	ft_env(t_all *all);
void	ft_unset_co(t_all *all, char *var);
void	ft_unset(t_all *all, t_cmd *cmd);
void	ft_exit(t_all *all, t_cmd *cmd);

/*				parse				*/

void    parse(t_all *all, char **line_mask);
void	get_colons(t_all *all, char *line, char *ref_line);
void	get_pips(t_all *all, char **line_mask);
void	free_all(t_all *all);
void	find_var(t_all *all, char **line_mask);
void	handle_great(t_all *all, char **line_mask, int *i);
void	handle_less(t_all *all, char **line_mask, int *i);
void    parse_heredoc(t_all *all, char **line_mask, int expand);
/*				make_ref			*/

int		cor_char(char c);
void	skip_back_s(t_all *all, char *mask, int *i);
void	skip_back_s_in_q(t_all *all, char *mask, int *i);
int		is_char_from_set(char c, char *set);
char	**remove_zero_ref(char **line_mask);

/*				handle quotes		*/

void	handle_s_quotes(t_all *all, char **line_mask, int *i);
void	handle_d_quotes(t_all *all, char **line_mask, int *i);
void	handle_var(char **line_mask, int *i);
void	handle_var_in_q(char **line_mask, int *i);

/*				env					*/

void	parce_env(t_all *all, char **env);
char	*check_env(t_list *list, char *str);

/*				cmd					*/

int		check_cmd(t_cmd *cmd);
int		args_nb(char *ref);
void	get_cmd(t_all *all, char **line_mask);
void	execute_cmd(t_all *all, t_cmd *cmd);
void	new_func(t_all *all, t_cmd *cmd);

/*				fd_files			*/

void	fd_files(t_all *all, t_cmd *cmd);

/*				helpers				*/
char	*ft_strjoinchar(char *s, char c);
int		str_n_char(char *str, char c);
void	free_content(void *content);
void	double_char_to_list(t_list **list, char **str);
void	prev_node(t_dlist **list);
void	next_node(t_dlist **list);
void	lstadd_dlist(t_dlist **alst, t_dlist *new);
t_dlist	*lstnewc(void *cam);
void	free_char(char **s);
int		skip_char(char *str, char c);
int		skip_space(char *str);

/*				history				*/
void	save_history(t_all *all, char *line);

/*				terminal			*/
int	terminal(t_all *all, char **line, char *promt);
int		init_term(t_tc_cmd *tccmd);

/*				path				*/
char	*find_path(t_all *all);
void	check_path(t_all *all);

/*				system				*/
char	**s_paths(t_all *all);
void	s_perror(t_all *all, char *name, int err);
void	s_wait(t_all *all, t_cmd *cmd);
char	*s_join(char *name, char c, char *val);
char	**s_env(t_all *all);
void	s_found(t_all *all, struct stat *st, char *file, t_cmd *cmd);
void	s_exec(t_all *all, t_cmd *cmd);
void	s_cmd(t_all *all, t_cmd *cmd);
void	s_last(t_all *all, t_cmd *cmd);
void	s_heredoc(t_all *all, t_cmd *cmd);

char	**s_readline(t_all *all, char *prompt);
void	s_cmd_files(t_cmd *cmd);

char	***ft_split_mask(char **line_mask, char c);
int		is_char_from_set(char c, char *set);
int		str_n_set(char *str, char *set);
void	s_dup(t_cmd *cmd);
#endif
