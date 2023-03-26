/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 04:38:29 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/01/15 23:08:36 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int char_check(char c)
{
	if(c=='|' || c=='<' || c=='>' || c=='&'|| c==' ')
	{
		return (1);
	}
	return (0);
}

void	ft_subadd(char *str,t_list **list)
{
	int	i;
	int quote;
	int dquote;
	i = 0;
	quote = 0; 
	dquote = 0;
	while (str[i])
	{
		while (*str == ' ')
		{
			str++;
			if(!*str)
				return ;
		}

		if( str[i]=='\"' && !(quote % 2))
			dquote++;
		else if( str[i]=='\'' && !(dquote % 2))
			quote++;
		
		if(!str[i + 1]||( char_check(str[i]) && !(quote % 2)&&!(dquote % 2)))
		{
			if(i == 0 && str[i] != ' ' )
			{
				ft_lstadd_back(list,ft_lstnew(ft_substr(str,0,1 + char_check(str[i + 1]))));
				str = &str[i + char_check(str[i + 1]) + 1];
			}else if(i != 0)
			{
			ft_lstadd_back(list,ft_lstnew(ft_substr(str,0,i + (!str[i + 1] && !char_check(str[i])))));
			str = &str[i + (!str[i + 1] && !char_check(str[i]))];
			
			}
			i = -1;
		}
		i++;
	}
	if((quote % 2)|(dquote % 2))
	{
		exit(1);
	}
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	slen;

	i = 0;
	if (!s)
		return (0);
	slen = ft_strlen(s);
	if (slen - 1 < start || slen == 0)
		return ("");
	if (slen - (size_t) start < len)
		len = slen -(size_t) start;
	str = malloc(len + 1);
	if (!str)
		return (0);
	while ((s + start)[i] && i < len)
	{
		str[i] = (s + start)[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_putstr(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

t_list	*ft_lstnew(char *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	if(!ft_strcmp(content,">>"))
		node->token = TOKEN_APPEND;
	else if(!ft_strcmp(content,"<<"))
		node->token = TOKEN_HEREDOC;
	else if(!ft_strcmp(content,"|"))
		node->token = TOKEN_PIPE;
	else if(!ft_strcmp(content,"||"))
		node->token = TOKEN_DPIPE;
	else if(!ft_strcmp(content,"<"))
		node->token = TOKEN_INPUT;
	else if(!ft_strcmp(content,">"))
		node->token = TOKEN_OUTPUT;
	else
		node->token = TOKEN_WORD;
	node->content = content;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*node;

	node = *(lst);
	if (!new)
		return ;
	if (*(lst))
	{
		while (node->next)
			node = node->next;
		node->next = new;
		new->prev = node;
		if(new->prev && new->token == TOKEN_WORD)
		{
			if(new->prev->token == TOKEN_APPEND)
				new->token = FILE_APP;
			else if(new->prev->token == TOKEN_INPUT)
				node->token = FILE_IN;
			else if(new->prev->token == TOKEN_OUTPUT)
				new->token = FILE_OUT;
		}
	}
	else
		*(lst) = new;
}

/*
t_list	*ft_lstlast(t_list *lst)
{
	t_list	*node;

	if (!lst)
		return (NULL);
	node = lst;
	while (node->next != NULL)
	{
		node = node->next;
	}
	return (node);
}
*/
/*
void	ft_freelist(t_list *lst)
{
	t_list	*node;

	if (!lst)
		return ;
	while (lst)
	{
		node = lst->next;
		free(lst);
		lst = lst->next;
	}
}*/