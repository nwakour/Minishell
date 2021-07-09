#include "minishell.h"

char	*find_path(t_all *all)
{
	t_list *tmp;

	tmp = all->l_env;
	while (tmp)
	{
		if (!(ft_strcmp(((t_env*)tmp->content)->name, "PATH")))
			return (ft_strdup(((t_env*)tmp->content)->value));
		tmp = tmp->next;
	}
	return NULL;
}

char	**s_paths(t_all *all)
{
	char **path;

	path = ft_split(find_path(all), ':');
	return (path);
}