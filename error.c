/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 08:56:47 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/04/15 08:56:49 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_check(t_list *list)
{
	while (list)
	{
		if (list->token == TOKEN_PIPE && (!list->next || !list->prev))
            return (ft_putstr("syntax error near unexpected token `|'",2), 1);
		else if (list->token == TOKEN_INPUT || list->token == TOKEN_APPEND
				|| list->token == TOKEN_OUTPUT || list->token == TOKEN_HEREDOC)
		{
			if (list->next)
			{
				if (list->next->token != FILE_IN
					&& list->next->token != FILE_OUT
					&& list->next->token != FILE_APP
					&& list->next->token != WORD_LIM)
					return (ft_putstr("syntax error near unexpected token `",
							2), ft_putstr(list->next->content, 2),
						ft_putstr("'", 2), exit_status = 258, 1);
			}
			else
				return (ft_putstr("syntax error near unexpected token `newline'",
						2), (0));
		}
		list = list->next;
	}
	return (0);
}
int	ft_check_pipe(t_list *list)
{
}
int	ft_check_redir(t_list *list)
{
	if (list->next)
	{
		if (list->next->token != FILE_IN && list->next->token != FILE_OUT
			&& list->next->token != FILE_APP && list->next->token != WORD_LIM)
		{
			ft_putstr("syntax error near unexpected token `", 2);
			ft_putstr(list->next->content, 2);
			ft_putstr("'", 2);
			exit_status = 258;
			return (1);
		}
		else
		{
			return (0);
		}
	}
	else
		return (ft_putstr("syntax error near unexpected token `newline'", 2),
				(0));
}
