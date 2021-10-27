/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:59:03 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/27 15:52:26 by hmahjour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <termcap.h>
# include <termios.h>
# include <signal.h>
# include <errno.h>
# include <sys/wait.h>
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

typedef struct s_cmd
{
	char	*cmd;
	char	**arg;
	char	**f_name;
	int		infd;
	int		fd;
	char	*in;
	char	*out;
	int		tmpin;
	int		tmpout;
	int		append;
	int		link;
	int		args;
	int		valid;
	int		pid;
	int		exec;
}t_cmd;

typedef struct s_line
{
	char	*line_mask[2];
}t_line;

typedef struct s_env
{
	char	*name;
	char	*value;
}t_env;

typedef struct s_dlist
{
	void			*cam;
	struct s_dlist	*prev;
	struct s_dlist	*next;
}t_dlist;

typedef struct s_all
{
	t_list	*l_cmd;
	t_list	*l_env;
	t_env	*env;
	t_cmd	*cmd;
	char	**envp;
	int		empty_cmd;
	int		error;
	int		exits;
	int		pip;
	int		inx;
	int		nextin;
	int		hdoc;
	int		add;
}					t_all;

/*				builtins			*/
void	ft_echo(t_all *all, char **arg);
t_list	*search_lst(t_list *list, t_env *var);
void	ft_export_wa(t_all *all, t_env *var);
void	ft_export(t_all *all, t_cmd *cmd);
void	ft_unset_co(t_all *all, char *var);
void	ft_cd(t_all *all, char *path, int args);
void	ft_unset(t_all *all, t_cmd *cmd);
void	ft_pwd(t_all *all);
void	ft_env(t_all *all);
void	ft_exit(t_all *all, t_cmd *cmd);
void	alpha_sort(t_list *list);
void	print_export(t_list *list);
void	ft_unset_co(t_all *all, char *var);

/*				cmd					*/
t_line	*ft_split_mask(char **line_mask, char c);
void	get_cmd(t_all *all, char **line_mask);
void	execute_cmd(t_all *all, t_cmd *cmd);
char	*s_readdoc(t_all *all, char *limit, int expand);

/*				parse				*/
void	parse(t_all *all, char **mline, int i, int ret);
void	get_pips(t_all *all, char **line_mask);
void	find_var(t_all *all, char **line_mask);
void	parse_heredoc(t_all *all, char **line_mask, int expand);

/*				env					*/
void	parce_env(t_all *all, char **env);
char	*check_env(t_list *list, char *str);
int		export_parse(t_all *all, char *s);
/*				fd_files			*/

void	fd_files(t_all *all, t_cmd *cmd);

/*				handle quotes		*/

void	handle_s_quotes(t_all *all, char **line_mask, int *i);
void	handle_d_quotes(t_all *all, char **line_mask, int *i);
void	handle_var(t_all *all, char **line_mask, int *i);
void	handle_var_in_q(char **line_mask, int *i);

/*				helpers				*/
int		str_n_set(char *str, char *set);
int		skip_space(char *str);
int		skip_char(char *str, char c);
char	*ft_strjoinchar(char *s, char c);
int		is_char_from_set(char c, char *set);
void	free_content(void *content);
void	free_array(char **array);
void	find_cmd(t_all *all, char **mline);
/*				make_ref			*/

void	skip_back_s(t_all *all, char *mask, int *i);
void	skip_back_s_in_q(t_all *all, char *mask, int *i);
void	remove_zero_ref(char **line_mask);

/*				path				*/
char	*find_path(t_all *all);
char	**s_paths(t_all *all);

/*				readline			*/
char	*s_readline(t_all *all, char *prompt, int doc);

/*				system				*/
void	s_perror(t_all *all, char *name, int err);
void	s_wait(t_all *all, t_cmd *cmd);
char	**s_env(t_all *all);
void	s_cmd(t_all *all, t_cmd *cmd);
void	s_last(t_all *all, t_cmd *cmd);
char	*s_join(char *name, char c, char *val);
void	s_exec(t_all *all, t_cmd *cmd);
char	**s_args(t_cmd *cmd);
void	s_found(t_all *all, struct stat *st, char *file, char **args);
t_all	*get_exit__ptr(t_all *exit);
void	get_files(int nextin, int nextout, t_cmd *cmd);
int		s_tern(int cond, int iftrue, int iffalse);
void	s_check_exec(t_all *all, t_cmd *cmd);
void	s_exec_most(t_all *all, char **tmp, char *file, struct stat *st);
void	s_exit_error(char *cmd, char *msg, int excode);
char	*update_path(t_all *all, int old);

#endif
