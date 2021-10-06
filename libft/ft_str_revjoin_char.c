/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_revjoin_char.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 15:50:44 by nwakour           #+#    #+#             */
/*   Updated: 2021/10/06 04:28:58 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_str_revjoin_char(char c, char *s)
{
	int		i;
	char	*str;

	i = 0;
	while (s && s[i])
		i++;
	str = (char *)malloc(sizeof(char) * (i + 2));
	if (!str)
		return (NULL);
	i = 0;
	str[i++] = c;
	while (s && s[i - 1])
	{
		str[i] = s[i - 1];
		i++;
	}
	str[i + 1] = '\0';
	if (s)
		free(s);
	return (str);
}
