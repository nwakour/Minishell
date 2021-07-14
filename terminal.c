/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 18:42:51 by nwakour           #+#    #+#             */
/*   Updated: 2021/07/14 13:12:15 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int ft_putc(int c)
{
	return write(1, &c, 1);
}

void printstr(char *s)
{
	int i;
	
	i = -1;
	while (s[++i])
		ft_putc(s[i]);
}

int ft_getlen(char *s)
{
	int i = -1;
	while (s[++i])
		;
	return (i);
}

/* Initialize new terminal i/o settings */
void initTermios(t_all *all)
{
	tgetent(NULL, getenv("TERM"));
	tcgetattr(0, all->old);			 /* grab old terminal i/o settings */
	all->new = all->old;					 /* make new settings same as old settings */
	all->new->c_lflag &= ~ICANON;		 /* disable buffered i/o */
	all->new->c_lflag &= ~ECHO;		 /* set echo mode */
	tcsetattr(0, TCSANOW, all->new); /* use these new terminal i/o settings now */
}

int		init_term(t_tc_cmd *tccmd)
{
    int ret;
	char *term_name = getenv("TERM");

	if (term_name == NULL)
	{
		fprintf(stderr, "Specify a terminal type with 'TERM=<type>'.\n");
		return -1;
	}

	ret = tgetent(NULL, term_name);

	if (ret == -1)
	{
		fprintf(stderr, "Could not access the termcap data base.\n");
		return -1;
	}
	else if (ret == 0)
	{
		fprintf(stderr, "Terminal type '%s' is not defined in termcap database (or too little information).\n", term_name);
		return -1;
	}
	tccmd->cl = tgetstr("cl", NULL);
	tccmd->le = tgetstr("le", NULL);
	tccmd->dl = tgetstr("dl", NULL);
	tccmd->nd = tgetstr("nd", NULL);
	return (0);
}
/* Restore old terminal i/o settings */
void resetTermios(t_all *all)
{
	tcsetattr(0, TCSANOW, all->old);
}
/* Read 1 character – echo defines echo mode */
char getch(t_all *all)
{
	char ch;
	initTermios(all);
	read(0, &ch, 1);
	resetTermios(all);
	return ch;
}

void c_Handler()
{
    // signal(SIGINT, c_Handler);
	write(1, "ctrl + c\nMinishell->>> ", 24);
}

void l_Handler(t_tc_cmd *tccmd)
{
	tputs(tccmd->cl, 1, ft_putc);
	write(1, "Minishell->>> ", 14);
}

void dd_Handler()
{
	// signal(SIGTERM, dd_Handler);
	write(1, "ctrl + dd\nMinishell->>> ", 25);
}

void aa_handler()
{
	// signal(SIGQUIT, aa_handler);
	write(1, "ctrl + aa\nMinishell->>> ", 25);
}

void z_handler()
{
	// signal(SIGTSTP, z_handler);
	write(1, "ctrl + z\nMinishell->>> ", 24);
}

// char *ch_line_content(char *line, char *new)
// {
// 	if (line[0])
// 		free(line);
// 	line = ft_strdup(new);
// 	return (line);
// }


int terminal(t_all *all, char **line, char *promt)
{
	char c;
	t_dlist *tmp;
	int help = 0;
	int edit = 0;
	char *s = "";
	
	signal(SIGINT, c_Handler);
	signal(SIGTSTP, z_handler);
	signal(SIGTERM, dd_Handler);
	signal(SIGQUIT, aa_handler);
	ft_putstr_fd(promt, 1);
	tmp = all->l_history;
	while (1)
	{
		c = getch(all);
		if (c == 4)
		{
			write(1, "ctrl + d\nMinishell->>> ", 24);
			continue ;
		}
		if (c == 12)
		{
			l_Handler(all->tccmd);
			continue ;
		}
		if (c == 27)
        {
			c = getch(all);
			c = getch(all);
			if (c == 'A')
			{
				tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_putc);
				tputs(all->tccmd->dl, 1, ft_putc);
				ft_putstr_fd(promt, 1);
				if (tmp)
				{
					if (!tmp->prev && !help)
					{
						s = (char*)tmp->cam;
						help = 1;
					}
					else
					{
						if(tmp->next)
							next_node(&tmp);
						s = (char*)tmp->cam;		
					}
					printstr(s);
				}
				edit = 0;
				continue ;
			}
			if (c == 'C')   
			{
				// tputs(all->tccmd->nd, 1, ft_putc);
				continue ;
			}
			if (c == 'B')
			{
				tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_putc);
				tputs(all->tccmd->dl, 1, ft_putc);
				ft_putstr_fd(promt, 1);
				if (tmp && tmp->prev)
				{
					prev_node(&tmp);
					s = (char*)tmp->cam;
				}
				else
				{
					s = *line;
					help = 0;
				}
				printstr(s);
				edit = 0;
				continue ;
			}
			if (c == 'D')
			{
				// tputs(all->tccmd->le, 1, ft_putc);
				continue ;
			}
            printf("%c\n", c);
        }
		else if (c == 127)
		{
			int l;
			
			if (!help)
			{
				l = ft_strlen(*line);
				if (l > 0)
				{
					tputs(all->tccmd->le, 1, ft_putc);
					tputs(tgetstr("dm", NULL), 1, ft_putc);
					tputs(tgetstr("dc", NULL), 1, ft_putc);
					tputs(tgetstr("ed", NULL), 1, ft_putc);
					tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_putc);
					ft_putstr_fd(promt, 1);
					(*line)[l - 1] = '\0';
						write(1, *line, l - 1);
				}
			}
			else
			{
				l = ft_strlen(s);
				if (l > 0)
				{
					tputs(all->tccmd->le, 1, ft_putc);
					tputs(tgetstr("dm", NULL), 1, ft_putc);
					tputs(tgetstr("dc", NULL), 1, ft_putc);
					tputs(tgetstr("ed", NULL), 1, ft_putc);
					tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_putc);
					ft_putstr_fd(promt, 1);
					if (!edit)
						s = ft_strdup(s);
					s[l - 1] = '\0';
						write(1, s, l - 1);
				}
			}
			edit = 1;
		}
		else if (c == 10)
		{
			if (tmp && help)
			{
				free(*line);
				if (edit)
					*line = s;
				else
					*line = ft_strdup(s);
			}
			write(1, "\n", 1);
			edit = 0;
			return (1);
		}
		else
		{
			write(1, &c, 1);
			if (!help)
				*line = ft_strjoinchar(*line, c);
			else
				s = ft_strjoinchar(s, c);
		}
	}
}