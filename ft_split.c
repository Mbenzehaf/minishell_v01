/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 18:53:23 by mben-zeh          #+#    #+#             */
/*   Updated: 2022/10/25 23:14:03 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	ft_count(const char *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c && str[i])
		i++;
		if (str[i] != c && str[i])
			count++;
		while (str[i] != c && str[i])
			i++;
	}
	return (count);
}

int	ft_worldlen(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

char	*ft_world(const char *s, char c)
{
	char	*str;
	int		i;
	int		worldlen;

	i = 0;
	worldlen = ft_worldlen(s, c);
	str = malloc(worldlen + 1);
	if (!str)
		return (0);
	while (s[i] && s[i] != c && i < worldlen)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

char	**ft_hyd(char **str)
{
	int	i;

	i = 0;
	while (!str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		count;
	int		i;

	if (!s)
		return (NULL);
	count = ft_count(s, c);
	i = 0;
	str = (char **)malloc((count + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	while (i < count)
	{
		while (*s == c && *s)
			s++;
		if (*s && *s != c)
		str[i] = ft_world(s, c);
		if (!str[i])
			return (ft_hyd(str));
		while (*s != c && *s)
			s++;
		i++;
	}
	str[i] = 0;
	return (str);
}
