/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 23:08:59 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/03/22 23:09:01 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

typedef enum {
    TOKEN_PIPE =  0,
    TOKEN_DPIPE = 1,
    TOKEN_APPEND = 2,
    TOKEN_HEREDOC = 3,
    TOKEN_INPUT = 4,
    TOKEN_OUTPUT = 5,
    TOKEN_WORD =  6,
    FILE_IN = 7,
    FILE_OUT = 8,
    FILE_APP = 9,
}TokenType;

typedef struct s_minishell
{
    int infile;
    int outfile;
    char **cmd;
}t_minishell;

typedef struct s_list
{
    int infile;
    int outfile;
    char *content;
    TokenType token;
    struct s_list *next;
    struct s_list *prev;
}t_list;

void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(char *content);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	ft_subadd(char *str,t_list **list);