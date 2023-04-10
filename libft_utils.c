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

int is_token(char c)
{
	return(c == '|' || c == '<' || c == '>' || c == '&' || c == ' '||c == '\t');
}

int ft_spl(char *str,t_list **list,t_env *env)
{
	int i;
	int quote;
	int dquote;

	quote = 0;
	dquote = 0;
	i = -1;
	while (str[++i])
	{
		while ((*str == ' '|| *str == '\t') && str++ )
			if(!*str)
				return (0);
		if( str[i]=='\"' && !(quote % 2))
			dquote++;
		else if( str[i]=='\'' && !(dquote % 2))
			quote++;
		if(str[i] && is_token(str[i]) && !(quote % 2) && !(dquote % 2))
		{
			if(i==0)
				while (is_token(str[i]) && str[i]==*str && i < 2)
					i++;
			ft_lstadd_back(list,ft_lstnew(ft_substr(str,0,i),env));
			str=&str[i];
			i = -1;
		}else if(!str[i + 1]&&!(quote % 2)&&!(dquote % 2))
		{
			ft_lstadd_back(list,ft_lstnew(ft_substr(str,0,i + (!str[i + 1] && str[i]!=' ')),env));
			str=&str[i + (!str[i + 1])];
			i = -1;
		}
	}
	return ((quote % 2)|(dquote % 2));
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
		return (NULL);
	slen = ft_strlen(s);
	if (slen - 1 < start || slen == 0)
		return (NULL);
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

t_list	*ft_lstnew(char *content,t_env *env)
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
		{
			node->token = TOKEN_WORD;
			content = ft_expand_quote(content,env);
			//printf("(%s)\n",content);
			//content = ft_expand_and_quote(content);
		}
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
		/*if(new->prev && new->token == TOKEN_WORD)
		{
			if(new->prev->token == TOKEN_APPEND)
				new->token = FILE_APP;
			else if(new->prev->token == TOKEN_INPUT)
				node->token = FILE_IN;
			else if(new->prev->token == TOKEN_OUTPUT)
				new->token = FILE_OUT;
		}*/
		if(node && new->token == TOKEN_WORD)
		{
			if(node->token == TOKEN_APPEND)
				new->token = FILE_APP;
			else if(node->token == TOKEN_INPUT)
				new->token = FILE_IN;
			else if(node->token == TOKEN_OUTPUT)
				new->token = FILE_OUT;
			else if(node->token == TOKEN_HEREDOC)
				new->token = WORD_LIM;
		}
	}
	else
		*(lst) = new;
}

t_data	*ft_dtnew(char **arg,int in,int out)
{
	t_data	*node;

	node = malloc(sizeof(t_data));
	if (!node)
		return (NULL);
	node->arg = arg;
	node->fdin = in;
	node->fdout = out;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	ft_dtadd_back(t_data **lst, t_data *new)
{
	t_data	*node;

	node = *(lst);
	if (!new)
		return ;
	if (*(lst))
	{
		while (node->next)
			node = node->next;
			new->prev = node;

		node->next = new;
	}
	else
		*(lst) = new;
}

t_env	*ft_envnew(char *var,char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_data));
	if (!node)
		return (NULL);
	node->value = value;
	node->var = var;
	node->next = NULL;
	return (node);
}

void	ft_envadd_back(t_env **lst, t_env *new)
{
	t_env	*node;

	node = *(lst);
	if (!new)
		return ;
	if (*(lst))
	{
		while (node->next)
			node = node->next;
		node->next = new;
	}
	else
		*(lst) = new;
}

char	*ft_strdup(const char *s1)
{
	int		len;
	int		i;
	char	*str;

	len = ft_strlen(s1);
	i = 0;
	str = malloc(len + 1);
	if (!str)
		return (0);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

char	*ft_check(char const *s1, char const *s2)
{
	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else
		return (ft_strdup(s1));
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;

	i = 0;
	if (!s1 || !s2)
		return (ft_check(s1, s2));
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (0);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (*s2)
	{
		str[i] = *s2;
		s2++;
		i++;
	}
	str[i] = 0;
	free(s1);
	s1 = NULL;
	return (str);
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

void	ft_freelist(t_list *lst)
{
	t_list	*node;

	if (!lst)
		return ;
	while (lst)
	{
		node = lst->next;
		free(lst->content);
		free(lst);
		lst = node;
	}
}

void	ft_freedata(t_data *lst)
{
	t_data	*node;
	int i;

	i = 0;
	node = NULL;
	if (!lst)
		return ;
	while (lst)
	{
		node = lst->next;
		while (lst->arg && lst->arg[i])
			i++;
		while(i)
			free(lst->arg[--i]);
		free(lst->arg);
		lst->arg = NULL;
		free(lst);
		lst = node;
	}
}
int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
	{
		return (1);
	}
	return (0);
}
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

int	ft_nbrlen(long n)
{
	int	count;

	count = 0;
	if (n <= 0)
	{
		count++;
		n *= (-1);
	}
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long		r;
	char		*str;
	int			len;
	int			sign;

	r = n;
	sign = 0;
	len = ft_nbrlen(r);
	str = malloc(len + 1);
	if (!str)
		return (0);
	str[len] = 0;
	if (r < 0)
	{
		str[0] = '-';
		r *= (-1);
		len--;
		sign++;
	}
	while (len > 0)
	{
		str[len-- + sign - 1] = r % 10 + 48;
		r /= 10;
	}
	return (str);
}