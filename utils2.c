/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <mben-zeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 00:51:30 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/04/22 09:24:20 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_check_n(char *str)
{
    int i;

    i = 0;
    if(str[i++]!='-')
        return (1);
    while (str[i])
    {
        if(str[i]!='n')
            return (1);
        i++;
    }
    return(0);
}

void ft_echo(char **str)
{
    int n;
    int i;

    n = 0;
    while (!ft_check_n(str[n]))
        n++;
    i = n;
    while (str[i])
    {
        ft_putstr(str[i],1);
        if(str[i + 1])
            ft_putstr(" ",1);
        i++;
    }
    if(!n)
        ft_putstr("\n",1);
}

void ft_cd(char *str)
{

}

void ft_export(t_env *env, char **str)
{

}

void ft_unset(t_env *env, char *str)
{
    t_env *node;

    while (env)
    {
        if(env->next && !ft_strcmp(env->next->var,str))
        {
            
            break ;
        }
        env = env->next;
    }
    
}

void ft_env(t_env *env,int is_export)
{
    while (env)
    {
        ft_putstr(env->var,1);
        if(is_export || env->value)
        {
            ft_putstr("=",1);
            ft_putstr(env->value,1);
        }
        env = env->next;
    }   
}
//◦ echo with option -n
//◦ cd with only a relative or absolute path ◦ pwd with no options
//◦ export with no options
//◦ unset with no options
//◦ env with no options or arguments
//◦ exit with no options
